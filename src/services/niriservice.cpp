#include "niriservice.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <qdebug.h>
#include <qjsonarray.h>

NiriService *NiriService::instance()
{
    return m_instance;
}

NiriService *NiriService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new NiriService(parent);
    return m_instance;
}

void NiriService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

QList<NiriWorkspace> NiriService::workspaces() const
{
    return m_workspaces;
}

void NiriService::focusWorkspace(quint64 id)
{
    sendIpcCommand(QString(R"({"Action":{"FocusWorkspace":{"reference":{"Id":%1}}}})").arg(id).toUtf8());
}

void NiriService::refreshClients()
{
    sendIpcCommand(R"({"Windows":null})");
}

QList<NiriClient> NiriService::clients() const
{
    return m_clients;
}

void NiriService::focusClient(quint64 id)
{
    sendIpcCommand(QString(R"({"Action":{"FocusWindow":{"id":%1}}})").arg(id).toUtf8());
}

void NiriService::refreshWorkspaces()
{
    sendIpcCommand(R"({"Workspaces":null})");
}

NiriService::NiriService(QObject *parent)
    : QObject{parent}
{
    commandSocket = new QLocalSocket(this);

    auto path = QString::fromLocal8Bit(qgetenv("NIRI_SOCKET"));
    connect(commandSocket, &QLocalSocket::readyRead, this, &NiriService::onCommandReadReady);
    commandSocket->connectToServer(path);

    eventSocket = new QLocalSocket(this);
    connect(eventSocket, &QLocalSocket::connected, this, [this]{
        const QByteArray request = R"({"EventStream":null})" "\n";
        eventSocket->write(request);
        eventSocket->flush();
    });
    connect(eventSocket, &QLocalSocket::readyRead, this, &NiriService::onEventReadReady);
    eventSocket->connectToServer(path);
}

NiriService::~NiriService()
{
    commandSocket->disconnect();
}

void NiriService::sendIpcCommand(const QByteArray &command)
{
    if(commandSocket->state() != QLocalSocket::ConnectedState)
    {
        qWarning() << "Niri IPC commandSocket is not connected";
        return;
    }
    QByteArray message = command;
    if(!message.endsWith('\n'))
        message.append('\n');

    commandSocket->write(message);
}

void NiriService::processCommandResponse(const QJsonObject &object)
{
    if(object.contains("Ok") && object.value("Ok").isObject())
    {
        const auto& obj = object.value("Ok").toObject();
        if(obj.contains("Workspaces"))
        {
            const auto workspacesArray = obj.value("Workspaces").toArray();
            m_workspaces.clear();
            for(const auto& ws : workspacesArray)
            {
                const auto wsObj = ws.toObject();
                NiriWorkspace workspace;
                workspace.id = wsObj.value("id").toInteger();
                workspace.idx = wsObj.value("idx").toInteger();
                workspace.isActive = wsObj.value("is_active").toBool();
                workspace.isFocused = wsObj.value("is_focused").toBool();
                workspace.isUrgent = wsObj.value("is_urgent").toBool();
                workspace.activeWindowId = wsObj.value("active_window_id").isNull() ? std::nullopt : std::optional<quint64>{wsObj.value("active_window_id").toInteger()};
                workspace.name = wsObj.value("name").toString();
                workspace.output = wsObj.value("output").toString();
                m_workspaces.append(workspace);
            }
            std::sort(m_workspaces.begin(), m_workspaces.end(), [](const auto& a, const auto& b){return a.idx < b.idx;});
            emit workspacesRefreshed();
            return;
        }
        if(obj.contains("Windows"))
        {
            m_clients.clear();
            const auto array = obj.value("Windows").toArray();
            bool hasActivce = false;
            for(const auto& co : array)
            {
                const auto object = co.toObject();
                NiriClient client;
                client.id = object.value("id").toInteger();
                client.appId = object.value("app_id").toString();
                client.isFloating = object.value("is_floating").toBool();
                client.isFocused = object.value("is_focused").toBool();
                client.workspaceId = object.value("workspace_id").toInteger();
                client.title = object.value("title").toString();
                client.isUrgent = object.value("is_urgent").toBool();
                if(!hasActivce && client.isFocused)
                    hasActivce = true;
                m_clients.append(client);
            }
            if(!hasActivce)
                emit noActiveWindow();

            emit clientsRefreshed();
            return;
        }
    }
}

void NiriService::processEventResponse(const QJsonObject &object)
{
    if(object.contains("WorkspaceActivated"))
    {
        const QJsonObject value = object.value("WorkspaceActivated").toObject();
        emit workspaceActivated(value.value("id").toInteger());
        return;
    }
    if(object.contains("WorkspacesChanged"))
    {
        const auto workspacesArray = object.value("WorkspacesChanged").toObject().value("workspaces").toArray();
        m_workspaces.clear();
        for(const auto& ws : workspacesArray)
        {
            const auto wsObj = ws.toObject();
            NiriWorkspace workspace;
            workspace.id = wsObj.value("id").toInteger();
            workspace.idx = wsObj.value("idx").toInteger();
            workspace.isActive = wsObj.value("is_active").toBool();
            workspace.isFocused = wsObj.value("is_focused").toBool();
            workspace.isUrgent = wsObj.value("is_urgent").toBool();
            workspace.activeWindowId = wsObj.value("active_window_id").isNull() ? std::nullopt : std::optional<quint64>{wsObj.value("active_window_id").toInteger()};
            workspace.name = wsObj.value("name").toString();
            workspace.output = wsObj.value("output").toString();
            m_workspaces.append(workspace);
        }
        std::sort(m_workspaces.begin(), m_workspaces.end(), [](const auto& a, const auto& b){return a.id < b.id;});
        emit workspacesRefreshed();
        return;
    }
    if(object.contains("WindowFocusChanged"))
    {
        const auto id = object.value("WindowFocusChanged").toObject().value("id").toInteger();
        bool hasActivce = false;
        for(auto& client : m_clients)
        {
            if(client.id == id)
            {
                client.isFocused = true;
                emit clientOpenedOrChanged(client);
            }
            if(!hasActivce && client.isFocused)
                hasActivce = true;
            client.isFocused = false;
        }
        if(!hasActivce)
            emit noActiveWindow();

        emit clientsRefreshed();
        emit clientFocused(id);
        return;
    }
    if(object.contains("WindowOpenedOrChanged"))
    {
        const auto obj = object.value("WindowOpenedOrChanged").toObject().value("window").toObject();
        NiriClient client;
        client.id = obj.value("id").toInteger();
        client.appId = obj.value("app_id").toString();
        client.isFloating = obj.value("is_floating").toBool();
        client.isFocused = obj.value("is_focused").toBool();
        client.workspaceId = obj.value("workspace_id").toInteger();
        client.title = obj.value("title").toString();
        client.isUrgent = obj.value("is_urgent").toBool();
        auto found = std::find_if(m_clients.begin(), m_clients.end(), [&client](const auto& c){return c.id == client.id;});
        if(found != m_clients.end())
        {
            found->appId = client.appId;
            found->workspaceId = client.workspaceId;
            found->title = client.title;
            found->isFloating = client.isFloating;
            found->isFocused = client.isFocused;
            found->isUrgent = client.isUrgent;
        }
        else
        {
            m_clients.append(client);
        }
        emit clientsRefreshed();
        emit clientOpenedOrChanged(client);
        return;
    }
    if(object.contains("WorkspaceActiveWindowChanged"))
    {
        const auto id = object.value("WorkspaceActiveWindowChanged").toObject().value("active_window_id").toInteger();
        bool hasActivce = false;
        for(auto& client : m_clients)
        {
            client.isFocused = (client.id == id);
            if(!hasActivce && client.isFocused)
                hasActivce = true;
        }
        if(!hasActivce)
            emit noActiveWindow();

        emit clientsRefreshed();
        emit clientFocused(id);
        return;
    }
    if(object.contains("WindowClosed"))
    {
        const auto id = object.value("WindowClosed").toObject().value("id").toInteger();
        m_clients.removeIf([id](const auto& c){return c.id == id;});
        emit clientsRefreshed();
        return;
    }
}

QList<QJsonObject> NiriService::parseJsonLines(QByteArray &buffer)
{
    QList<QJsonObject> result;
    while(true)
    {
        const qsizetype newline = buffer.indexOf('\n');
        if(newline < 0)
            break;

        const QByteArray line = buffer.left(newline);
        buffer.remove(0, newline + 1);

        if(line.trimmed().isEmpty())
            continue;

        QJsonParseError error;
        const QJsonDocument document = QJsonDocument::fromJson(line, &error);

        if(error.error != QJsonParseError::NoError)
        {
            qWarning() << "Invalid Niri JSON:" << error.errorString() << line;
            continue;
        }

        if(document.isObject())
            result.append(document.object());
    }

    return result;
}

void NiriService::onCommandReadReady()
{
    m_commandReadBuffer.append(commandSocket->readAll());
    for(const auto& response : parseJsonLines(m_commandReadBuffer))
    {
        processCommandResponse(response);
    }
}

void NiriService::onEventReadReady()
{
    m_eventReadBuffer.append(eventSocket->readAll());
    for(const auto& event : parseJsonLines(m_eventReadBuffer))
    {
        processEventResponse(event);
    }
}

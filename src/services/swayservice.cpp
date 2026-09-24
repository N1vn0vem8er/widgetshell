#include "swayservice.h"
#include <qdebug.h>
#include <qjsonarray.h>
#include <qjsonobject.h>

SwayService *SwayService::instance()
{
    return m_instance;
}

SwayService *SwayService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new SwayService(parent);
    return m_instance;
}

void SwayService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

QList<SwayWorkspace> SwayService::workspaces() const
{
    return m_workspaces;
}

void SwayService::refreshWorkspaces()
{
    sendIpcCommand(GET_WORKSPACES, {});
}

void SwayService::refreshClients()
{
    sendIpcCommand(GET_TREE, {});
}

void SwayService::focusClient(int id)
{
    sendIpcCommand(RUN_COMMAND, QString("[con_id=%1] focus").arg(id).toUtf8());
}

void SwayService::focusWorkspace(const QString &name)
{
    sendIpcCommand(RUN_COMMAND, QString("workspace \"%1\"").arg(name).toUtf8());
}

void SwayService::updateWorkspaces(const QJsonArray &array)
{
    QList<SwayWorkspace> workspaces;
    workspaces.reserve(array.size());

    for(const auto &value : array)
    {
        if(!value.isObject())
            continue;
        workspaces.append(parseWorkspace(value.toObject()));
    }

    m_workspaces = std::move(workspaces);

    emit workspacesChanged();
}

SwayWorkspace SwayService::parseWorkspace(const QJsonObject &object) const
{
    SwayWorkspace workspace;
    workspace.id = object.value("id").toInt();
    workspace.number = object.value("num").toInt(-1);
    workspace.name = object.value("name").toString();
    workspace.output = object.value("output").toString();
    workspace.focused = object.value("focused").toBool();
    workspace.visible = object.value("visible").toBool();
    workspace.urgent = object.value("urgent").toBool();
    return workspace;
}

SwayClient SwayService::parseClient(const QJsonObject &object) const
{
    SwayClient client;
    client.id = object.value("id").toInt();
    client.pid = object.value("pid").toInt(-1);
    client.appId = object.value("app_id").toString();
    client.title = object.value("name").toString();
    client.focused = object.value("focused").toBool();
    client.urgent = object.value("urgent").toBool();
    const QJsonObject properties = object.value("window_properties").toObject();
    client.windowClass = properties.value("class").toString();
    client.windowInstance = properties.value("instance").toString();
    return client;
}

void SwayService::handleWindowEvent(const QJsonObject &event)
{
    const QString change = event.value("change").toString();
    const QJsonObject container = event.value("container").toObject();
    const int id = container.value("id").toInt();

    if(id == 0)
    {
        qWarning() << "Window event without container id";
        return;
    }

    if(change == "new")
    {
        const SwayClient client = parseClient(container);
        m_clients.append(client);
        emit clientOpened(client);
    }
    else if(change == "close")
    {
        m_clients.removeIf([id](const auto client){return id == client.id;});
        emit clientClosed(id);
    }
    else if(change == "focus" || change == "title" || change == "move")
    {
        refreshClients();
    }
}

void SwayService::updateClients(const QJsonObject &tree)
{
    QList<SwayClient> clients;
    collectClients(tree, clients, {}, {});
    m_clients = std::move(clients);
    emit clientsChanged();
}

void SwayService::collectClients(const QJsonObject &node, QList<SwayClient> &clients, const QString &workspace, const QString &output)
{
    const QString type = node["type"].toString();
    QString currentWorkspace = workspace;
    QString currentOutput = output;

    if(type == "output")
        currentOutput = node["name"].toString();
    else if(type == "workspace")
        currentWorkspace = node["name"].toString();

    const QJsonArray nodes = node["nodes"].toArray();
    const QJsonArray floatingNodes = node["floating_nodes"].toArray();

    if(type == "con" && nodes.isEmpty() && floatingNodes.isEmpty())
    {
        SwayClient client = parseClient(node);
        if(client.focused)
            emit clientFocused(client.id);
        client.workspace = currentWorkspace;
        client.output = currentOutput;
        clients.append(client);
    }

    for(const QJsonValue& value : nodes)
    {
        collectClients(value.toObject(), clients, currentWorkspace, currentOutput);
    }

    for(const QJsonValue& value : floatingNodes)
    {
        collectClients(value.toObject(), clients, currentWorkspace, currentOutput);
    }
}

SwayService::SwayService(QObject *parent)
    : QObject{parent}
{
    socket2 = new QLocalSocket(this);
    auto path = QString::fromLocal8Bit(qgetenv("SWAYSOCK"));

    if(path.isEmpty())
    {
        qCritical() << "SWAYSOCK not found";
        return;
    }

    connect(socket2, &QLocalSocket::readyRead, this, &SwayService::onReadReady);
    connect(socket2, &QLocalSocket::connected, this, &SwayService::onConnected);
    connect(socket2, &QLocalSocket::disconnected, this, &SwayService::onDisconnected);
    connect(socket2, &QLocalSocket::errorOccurred, this, &SwayService::onError);

    socket2->connectToServer(path);
}

SwayService::~SwayService()
{
    socket2->disconnect();
}

void SwayService::sendIpcCommand(quint32 type, const QByteArray &payload)
{
    if(socket2->state() != QLocalSocket::ConnectedState)
    {
        qWarning() << "Sway IPC socket is not connected";
        return;
    }

    QByteArray message;
    message.reserve(14 + payload.size());
    message.append("i3-ipc");

    const quint32 length = payload.size();
    message.append(char(length & 0xff));
    message.append(char((length >> 8) & 0xff));
    message.append(char((length >> 16) & 0xff));
    message.append(char((length >> 24) & 0xff));
    message.append(char(type & 0xff));
    message.append(char((type >> 8) & 0xff));
    message.append(char((type >> 16) & 0xff));
    message.append(char((type >> 24) & 0xff));
    message.append(payload);

    socket2->write(message);
}

void SwayService::subscribeToEvents()
{
    const QJsonArray events {"workspace", "window"};

    const auto payload = QJsonDocument(events).toJson(QJsonDocument::Compact);

    sendIpcCommand(SUBSCRIBE, payload);
}

void SwayService::onReadReady()
{
    m_readBuffer.append(socket2->readAll());
    while(true)
    {
        if(m_readBuffer.size() < 14)
            return;

        if(m_readBuffer.left(6) != "i3-ipc")
        {
            qWarning() << "Invalid Sway IPC magic";
            const auto index = m_readBuffer.indexOf("i3-ipc", 1);
            if(index < 0)
            {
                m_readBuffer.clear();
                return;
            }

            m_readBuffer.remove(0, index);
            continue;
        }

        const auto readUint32LE = [](const QByteArray& data, qsizetype offset) -> quint32 {
            return static_cast<quint32>(static_cast<quint8>(data.at(offset))) |
                   (static_cast<quint32>(static_cast<quint8>(data.at(offset + 1))) << 8) |
                   (static_cast<quint32>(static_cast<quint8>(data.at(offset + 2))) << 16) |
                   (static_cast<quint32>(static_cast<quint8>(data.at(offset + 3))) << 24);
        };

        const quint32 payloadLength = readUint32LE(m_readBuffer, 6);
        const quint32 type = readUint32LE(m_readBuffer, 10);
        const qsizetype messageSize = 14 + payloadLength;

        if(m_readBuffer.size() < messageSize)
            return;

        const QByteArray payload = m_readBuffer.mid(14, payloadLength);

        m_readBuffer.remove(0, messageSize);

        const QJsonDocument json = QJsonDocument::fromJson(payload);

        if(json.isNull())
        {
            qWarning() << "Invalid json from Sway:" << payload;
            continue;
        }

        if(type == GET_WORKSPACES)
        {
            if(!json.isArray())
            {
                qWarning() << "GET_WORKSPACES returned non array";
                continue;
            }

            updateWorkspaces(json.array());
            continue;
        }
        else if(type == EVENT_WORKSPACE)
        {
            if(!json.isObject())
            {
                qWarning() << "Workspace event is not an object";
                continue;
            }

            refreshWorkspaces();
            continue;
        }
        else if(type == GET_TREE)
        {
            if(!json.isObject())
            {
                qWarning() << "GET_TREE returned non object";
                continue;
            }

            updateClients(json.object());
            continue;
        }
        else if(type == EVENT_WINDOW)
        {
            if(!json.isObject())
            {
                qWarning() << "Window event is not an object";
                continue;
            }

            handleWindowEvent(json.object());
            continue;
        }
    }
}

void SwayService::onConnected()
{
    qDebug() << "Connected to Sway IPC";
    refreshWorkspaces();
    refreshClients();
    subscribeToEvents();
}

void SwayService::onDisconnected()
{
    qDebug() << "Disconnected from Sway IPC";
}

void SwayService::onError()
{
    qWarning() << "Sway IPC error:" << socket2->errorString();
}

QList<SwayClient> SwayService::clients() const
{
    return m_clients;
}

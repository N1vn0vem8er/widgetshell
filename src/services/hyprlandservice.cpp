#include "hyprlandservice.h"
#include <QDebug>
#include <QJsonDocument>

HyprlandService *HyprlandService::instance()
{
    return m_instance;
}

HyprlandService *HyprlandService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new HyprlandService(parent);
    return m_instance;
}

void HyprlandService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

QList<HyprlandWorkspace> HyprlandService::getWorkspaces()
{
    QByteArray response = sendIpcCommand("j/workspaces");

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if(!doc.isArray())
        return {};

    QList<HyprlandWorkspace> workspaces;
    const QJsonArray array = doc.array();
    for(const auto& val : array)
    {
        QJsonObject obj = val.toObject();
        HyprlandWorkspace workspace;
        workspace.id = obj.value("id").toInt();
        workspace.name = obj.value("name").toString();
        workspace.windowsCount = obj.value("windows").toInt();
        workspace.hasFullscreen = obj.value("hasfullscreen").toBool();
        workspaces.append(workspace);
    }
    std::sort(workspaces.begin(), workspaces.end(), [](const auto& a, const auto& b){return a.id < b.id;});
    return workspaces;
}

void HyprlandService::switchWorkspace(int id)
{
    sendIpcCommand(QString("dispatch hl.dsp.focus({workspace = %1})").arg(id).toUtf8());
}

QList<HyprlandClient> HyprlandService::getClients()
{
    QByteArray response = sendIpcCommand("j/clients");

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if(!doc.isArray())
        return {};

    QList<HyprlandClient> clients;
    const QJsonArray array = doc.array();
    for(const auto& val : array)
    {
        QJsonObject obj = val.toObject();
        clients.append(HyprlandClient::fromJson(obj));
    }
    return clients;
}

HyprlandWorkspace HyprlandService::getWorkspace(int id)
{
    QByteArray response = sendIpcCommand("j/workspace/1");
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if(!doc.isArray())
        return {};

    HyprlandWorkspace workspace;
    return workspace;
}

void HyprlandService::focusWindow(quint64 address)
{
    sendIpcCommand(QString("dispatch hl.dsp.focus({ window = \"address:0x%1\" })").arg(address, 0, 16).toUtf8());
}

int HyprlandService::getActiveWorkspaceId()
{
    QByteArray response = sendIpcCommand("j/activeworkspace");

    QJsonObject obj = QJsonDocument::fromJson(response).object();
    return obj.value("id").toInt(-1);
}

HyprlandClient HyprlandService::getActiveClient()
{
    QByteArray response = sendIpcCommand("j/activewindow");
    QJsonObject obj = QJsonDocument::fromJson(response).object();
    return HyprlandClient::fromJson(obj);
}

HyprlandService::HyprlandService(QObject *parent)
    : QObject{parent}
{
    socket2 = new QLocalSocket(this);
    xdgRuntime = qgetenv("XDG_RUNTIME_DIR");
    his = qgetenv("HYPRLAND_INSTANCE_SIGNATURE");

    if(his.isEmpty())
    {
        qCritical() << "HYPRLAND_INSTANCE_SIGNATURE not found";
        return;
    }

    connect(socket2, &QLocalSocket::readyRead, this, &HyprlandService::onReadReady);

    socket2->connectToServer(QString("%1/hypr/%2/.socket2.sock").arg(xdgRuntime, his));
}

HyprlandService::~HyprlandService()
{
    if(socket2)
        socket2->close();
}

QByteArray HyprlandService::sendIpcCommand(const QByteArray &command)
{
    QLocalSocket socket;
    socket.connectToServer(QString("%1/hypr/%2/.socket.sock").arg(xdgRuntime, his));
    if(!socket.waitForConnected(1000))
    {
        qWarning() << "Cant connect to socket";
        return {};
    }

    socket.write(command);
    socket.flush();

    if(!socket.waitForReadyRead(1000))
        return {};

    QByteArray resp = socket.readAll();
    socket.close();
    return resp;
}

void HyprlandService::onReadReady()
{
    while(socket2->canReadLine())
    {
        QString line = QString::fromUtf8(socket2->readLine()).trimmed();
        if(line.startsWith("workspace>>"))
        {
            emit workspacesChanged();
            bool ok;
            int id = line.section(">>", 1).toInt(&ok);
            if(ok)
                emit workspaceChanged(id);
            else
                qCritical() << "Error parsing workspace id in workspace";
            continue;
        }
        if(line.startsWith("createworkspace>>"))
        {
            emit workspacesChanged();
            continue;
        }
        if(line.startsWith("destroyworkspace>>"))
        {
            emit workspacesChanged();
            continue;
        }
        if(line.startsWith("focusedmon>>"))
        {
            emit workspacesChanged();
            continue;
        }
        if(line.startsWith("openwindow"))
        {
            const QStringList parts = line.mid(QStringLiteral("openwindow>>").size()).split(',');
            if(parts.size() >= 4)
            {
                const QString windowAddress = parts.at(0);
                const QString workspaceName = parts.at(1);
                const QString windowClass = parts.at(2);
                const QString windowTitle = parts.mid(3).join(',');
                bool ok;
                quint64 address = windowAddress.toULongLong(&ok, 16);

                if(!ok)
                {
                    qCritical() << "Error parsing window address in openwindow";
                    continue;
                }
                emit openedWindow(address, workspaceName, windowClass, windowTitle);
                emit clientsChanged();
            }
            continue;
        }
        if(line.startsWith("closewindow"))
        {
            const QString windowAddress = line.section(">>", 1);
            bool ok;
            quint64 address = windowAddress.toULongLong(&ok, 16);

            if(!ok)
            {
                qCritical() << "Error parsing window address in closewindow";
                continue;
            }
            emit closedWindow(address);
            emit clientsChanged();
            continue;
        }
        if(line.startsWith("movewindow"))
        {
            const QStringList parts = line.mid(QStringLiteral("movewindow>>").size()).split(',');
            if(parts.size() >= 2)
            {
                const QString windowAddress = parts.at(0);
                const QString workspaceName = parts.at(1);
                bool ok;
                quint64 address = windowAddress.toULongLong(&ok, 16);

                if(!ok)
                {
                    qCritical() << "Error parsing window address in movewindow";
                    continue;
                }
                emit movedWindow(address, workspaceName);
                emit clientsChanged();
            }
            continue;
        }
        if(line.startsWith("movewindowv2"))
        {
            const QStringList parts = line.mid(QStringLiteral("movewindowv2>>").size()).split(',');
            if(parts.size() >= 3)
            {
                const QString windowAddress = parts.at(0);
                bool ok;
                const int workspaceId = parts.at(1).toInt(&ok);
                if(!ok)
                {
                    qCritical() << "Error parsing workspace id in movewindowv2";
                    continue;
                }
                const QString workspaceName = parts.at(2);
                quint64 address = windowAddress.toULongLong(&ok, 16);

                if(!ok)
                {
                    qCritical() << "Error parsing window address in movewindowv2";
                    continue;
                }
                emit movedWindowV2(address, workspaceId, workspaceName);
                emit clientsChanged();
            }
            continue;
        }
        if(line.startsWith("activewindowv2"))
        {
            const QString windowAddress = line.section(">>", 1);
            if(windowAddress.isEmpty())
            {
                emit activeWindowEmpty();
                continue;
            }
            bool ok;
            quint64 address = windowAddress.toULongLong(&ok, 16);

            if(!ok)
            {
                qCritical() << "Error parsing window address in activewindowv2";
                continue;
            }

            emit activedWindowV2(address);
            continue;
        }
        if(line.startsWith("activewindow"))
        {
            const QStringList parts = line.mid(QStringLiteral("activewindow>>").size()).split(',');
            if(parts.size() >= 2)
            {
                const QString windowClass = parts.at(0);
                const QString windowTitle = parts.at(1);
                emit activedWindow(windowClass, windowTitle);
            }
            continue;
        }
        if(line.startsWith("createworkspace"))
        {
            const QString workspaceName = line.section(">>", 1);
            emit createdWorkspace(workspaceName);
            continue;
        }
        if(line.startsWith("screencastv2"))
        {
            const QStringList parts = line.mid(QStringLiteral("screencastv2>>").size()).split(',');
            if(parts.size() >= 3)
            {
                bool ok;
                const int state = parts.at(0).toInt(&ok);

                if(!ok)
                {
                    qCritical() << "Error parsing state in screencastv2";
                    continue;
                }
                const QString owner = parts.at(1);
                const QString name = parts.at(2);
                emit screenCastedV2(state, owner, name);
            }
            continue;
        }
        if(line.startsWith("screencast"))
        {
            const QStringList parts = line.mid(QStringLiteral("screencast>>").size()).split(',');
            if(parts.size() >= 2)
            {
                bool ok;
                const int state = parts.at(0).toInt(&ok);

                if(!ok)
                {
                    qCritical() << "Error parsing state in screencast";
                    continue;
                }
                const QString owner = parts.at(1);
                emit screenCasted(state, owner);
            }
            continue;
        }
    }
}

#include "hyprlandservice.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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
    QLocalSocket socket;
    socket.connectToServer(QString("%1/hypr/%2/.socket.sock").arg(xdgRuntime, his));
    if(!socket.waitForConnected(1000))
    {
        qWarning << "Can't connect to socket";
        return {};
    }

    socket.write("j/workspaces");
    socket.flush();

    if(!socket.waitForReadyRead(1000))
        return {};

    QByteArray response = socket.readAll();
    socket.close();

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
    return workspaces;
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

void HyprlandService::onReadReady()
{
    while(socket2->canReadLine())
    {
        QString line = QString::fromUtf8(socket2->readLine()).trimmed();
        if(line.startsWith("workspace>>"))
        {
            bool ok;
            int id = line.section(">>", 1).toInt(&ok);
            if(ok)
                emit workspaceChanged(id);
            else
                qCritical() << "Error parsing workspace id";
        }
    }
}

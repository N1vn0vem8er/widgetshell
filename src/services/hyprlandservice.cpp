#include "hyprlandservice.h"
#include <QDebug>

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

HyprlandService::HyprlandService(QObject *parent)
    : QObject{parent}
{
    socket2 = new QLocalSocket(this);
    QString xdgRuntime = qgetenv("XDG_RUNTIME_DIR");
    QString his = qgetenv("HYPRLAND_INSTANCE_SIGNATURE");

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

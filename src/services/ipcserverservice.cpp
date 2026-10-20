#include "ipcserverservice.h"

#include <QApplication>
#include <QLocalSocket>

IPCServerService *IPCServerService::instance()
{
    return m_instance;
}

IPCServerService *IPCServerService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new IPCServerService(parent);
    return m_instance;
}

void IPCServerService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

IPCServerService::IPCServerService(QObject *parent)
    : QObject{parent}
{
    server = new QLocalServer(this);
    if(!server->listen("WIDGETSHELL"))
    {
        qWarning() << server->errorString();
        return;
    }
    connect(server, &QLocalServer::newConnection, this, &IPCServerService::handleConnections);
}

IPCServerService::~IPCServerService()
{
    server->removeServer("WIDGETSHELL");
}

void IPCServerService::handleConnections()
{
    while(server->hasPendingConnections())
    {
        QLocalSocket* socket = server->nextPendingConnection();
    }
}

#include "ipcserverservice.h"

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

}

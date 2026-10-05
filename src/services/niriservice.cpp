#include "niriservice.h"

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

NiriService::NiriService(QObject *parent)
    : QObject{parent}
{

}

QByteArray NiriService::sendIpcCommand(const QByteArray &command)
{

}

void NiriService::onReadReady()
{

}

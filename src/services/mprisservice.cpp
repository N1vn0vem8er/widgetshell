#include "mprisservice.h"

MprisService *MprisService::instance()
{
    return m_instance;
}

MprisService *MprisService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new MprisService(parent);
    return m_instance;
}

void MprisService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

MprisService::MprisService(QObject *parent)
    : QObject{parent}
{}

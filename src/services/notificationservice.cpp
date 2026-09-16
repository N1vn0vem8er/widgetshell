#include "notificationservice.h"

NotificationService *NotificationService::instance()
{
    return m_instance;
}

NotificationService *NotificationService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new NotificationService(parent);
    return m_instance;
}

void NotificationService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

NotificationService::NotificationService(QObject *parent)
    : QObject{parent}
{

}

NotificationService::~NotificationService()
{

}

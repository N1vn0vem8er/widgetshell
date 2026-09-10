#include "networkservice.h"
#include <qdebug.h>

NetworkService *NetworkService::instance()
{
    return m_instance;
}

NetworkService *NetworkService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new NetworkService(parent);
    return m_instance;
}

void NetworkService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

NetworkService::NetworkService(QObject *parent)
{
    m_networkInfo = QNetworkInformation::instance();
    if(!m_networkInfo)
        qWarning() << "Failed to get QNetworkInformation";
    else
    {
        m_reachability = m_networkInfo->reachability();
        m_transportMedium = m_networkInfo->transportMedium();
        connect(m_networkInfo, &QNetworkInformation::reachabilityChanged, this, &NetworkService::updateReachability);
        connect(m_networkInfo, &QNetworkInformation::transportMediumChanged, this, &NetworkService::updateTransportMedium);
    }
}

NetworkService::~NetworkService()
{

}

void NetworkService::updateReachability(QNetworkInformation::Reachability reachability)
{

}

void NetworkService::updateTransportMedium(QNetworkInformation::TransportMedium medium)
{

}



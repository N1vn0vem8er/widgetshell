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

bool NetworkService::isOnline() const
{
    return m_reachability == QNetworkInformation::Reachability::Online;
}

bool NetworkService::isWifi() const
{
    return m_transportMedium == QNetworkInformation::TransportMedium::WiFi;
}

QNetworkInformation::Reachability NetworkService::reachability() const
{
    return m_reachability;
}

QNetworkInformation::TransportMedium NetworkService::transportMedium() const
{
    return m_transportMedium;
}

NetworkService::NetworkService(QObject *parent)
{
    QNetworkInformation::loadDefaultBackend();
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
    if(m_reachability == reachability)
        return;
    const bool wasOnline = isOnline();
    m_reachability = reachability;

    emit reachabilityChanged(m_reachability);
    if(wasOnline != isOnline())
        emit onlineChanged(isOnline());
}

void NetworkService::updateTransportMedium(QNetworkInformation::TransportMedium medium)
{
    if(m_transportMedium == medium)
        return;
    const bool wasWifi = isWifi();
    m_transportMedium = medium;

    emit transportMediumChanged(m_transportMedium);
    if(wasWifi != isWifi())
        emit wifiChanged(isWifi());
}



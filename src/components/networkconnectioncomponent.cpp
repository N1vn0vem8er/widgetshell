#include "networkconnectioncomponent.h"
#include <qboxlayout.h>

NetworkConnectionComponent::NetworkConnectionComponent(QWidget *parent)
    : QWidget{parent}
{
    service = NetworkService::instance();
    connectionbutton = new QPushButton(this);
    connectionbutton->setFlat(true);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(connectionbutton);
    updateTransportMedium(service->transportMedium());
    updateReachability(service->reachability());
}

void NetworkConnectionComponent::updateIcons()
{
    switch(m_transportMedium)
    {

    case QNetworkInformation::TransportMedium::Unknown:
        switch(m_reachability)
        {
        case QNetworkInformation::Reachability::Unknown:
            connectionbutton->setIcon(QIcon::fromTheme("unknown"));
            break;
        case QNetworkInformation::Reachability::Disconnected:
        case QNetworkInformation::Reachability::Local:
        case QNetworkInformation::Reachability::Site:
            connectionbutton->setIcon(QIcon::fromTheme("network-disconnect"));
            break;
        case QNetworkInformation::Reachability::Online:
            connectionbutton->setIcon(QIcon::fromTheme("network-connect"));
            break;
        }
        break;
    case QNetworkInformation::TransportMedium::Ethernet:
    case QNetworkInformation::TransportMedium::Cellular:
        switch(m_reachability)
        {
        case QNetworkInformation::Reachability::Unknown:
            connectionbutton->setIcon(QIcon::fromTheme("network-wired-unavailable"));
            break;
        case QNetworkInformation::Reachability::Disconnected:
            connectionbutton->setIcon(QIcon::fromTheme("network-wired-disconnected"));
            break;
        case QNetworkInformation::Reachability::Local:
        case QNetworkInformation::Reachability::Site:
            connectionbutton->setIcon(QIcon::fromTheme("network-wired-activated-limited"));
            break;
        case QNetworkInformation::Reachability::Online:
            connectionbutton->setIcon(QIcon::fromTheme("network-wired"));
            break;
        }
        break;
    case QNetworkInformation::TransportMedium::WiFi:
        switch(m_reachability)
        {
        case QNetworkInformation::Reachability::Unknown:
            connectionbutton->setIcon(QIcon::fromTheme("network-wireless-connected-00"));
            break;
        case QNetworkInformation::Reachability::Disconnected:
            connectionbutton->setIcon(QIcon::fromTheme("network-wireless-disconnected"));
            break;
        case QNetworkInformation::Reachability::Local:
            case QNetworkInformation::Reachability::Site:
            connectionbutton->setIcon(QIcon::fromTheme("network-wireless-100-limited"));
            break;
        case QNetworkInformation::Reachability::Online:
            connectionbutton->setIcon(QIcon::fromTheme("network-wireless"));
            break;
        }
        break;
    case QNetworkInformation::TransportMedium::Bluetooth:
        switch(m_reachability)
        {
        case QNetworkInformation::Reachability::Unknown:
        case QNetworkInformation::Reachability::Disconnected:
            connectionbutton->setIcon(QIcon::fromTheme("network-bluetooth-inactive-symbolic"));
            break;
        case QNetworkInformation::Reachability::Local:
        case QNetworkInformation::Reachability::Site:
        case QNetworkInformation::Reachability::Online:
            connectionbutton->setIcon(QIcon::fromTheme("network-bluetooth"));
            break;
        }
        break;
    }
}

void NetworkConnectionComponent::updateTransportMedium(QNetworkInformation::TransportMedium transportMedium)
{
    m_transportMedium = transportMedium;
    updateIcons();
}

void NetworkConnectionComponent::updateReachability(QNetworkInformation::Reachability reachability)
{
    m_reachability = reachability;
    updateIcons();
}

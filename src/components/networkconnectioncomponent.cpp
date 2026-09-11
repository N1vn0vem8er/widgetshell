#include "networkconnectioncomponent.h"
#include <qboxlayout.h>

NetworkConnectionComponent::NetworkConnectionComponent(Unit unit, QWidget *parent)
    : unit(unit), QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    service = NetworkService::instance();
    connectionbutton = new QPushButton(this);
    connectionbutton->setFlat(true);
    networkDownloadLabel = new QLabel(this);
    networkUploadLabel = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(connectionbutton);
    layout->addWidget(networkDownloadLabel);
    layout->addWidget(networkUploadLabel);
    layout->setContentsMargins(4, 0, 4, 0);
    connect(service, &NetworkService::networkSpeed, this, &NetworkConnectionComponent::updateDownloadUpload);
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
    connectionbutton->setToolTip(createTooltipText(m_transportMedium, m_reachability));
}

QString NetworkConnectionComponent::createTooltipText(QNetworkInformation::TransportMedium transportMedium, QNetworkInformation::Reachability reachability) const
{
    QString tooltip;
    switch(transportMedium)
    {
    case QNetworkInformation::TransportMedium::Unknown:
        tooltip.append("TransportMedium: Unknown");
        break;
    case QNetworkInformation::TransportMedium::Ethernet:
        tooltip.append("TransportMedium: Ethernet");
        break;
    case QNetworkInformation::TransportMedium::Cellular:
        tooltip.append("TransportMedium: Cellular");
        break;
    case QNetworkInformation::TransportMedium::WiFi:
        tooltip.append("TransportMedium: WiFi");
        break;
    case QNetworkInformation::TransportMedium::Bluetooth:
        tooltip.append("TransportMedium: Bluetooth");
        break;
    }
    switch(reachability)
    {
    case QNetworkInformation::Reachability::Unknown:
        tooltip.append(" | Reachability: Unknown");
        break;
    case QNetworkInformation::Reachability::Disconnected:
        tooltip.append(" | Reachability: Disconnected");
        break;
    case QNetworkInformation::Reachability::Local:
        tooltip.append(" | Reachability: Local");
        break;
    case QNetworkInformation::Reachability::Site:
        tooltip.append(" | Reachability: Site");
        break;
    case QNetworkInformation::Reachability::Online:
        tooltip.append(" | Reachability: Online");
        break;
    }
    return tooltip;
}

QString NetworkConnectionComponent::formatSpeed(double bytesPerSecond) const
{
    switch(unit)
    {
    case Unit::B:
        break;
    case Unit::KB:
        bytesPerSecond /= 1024.0;
        break;
    case Unit::MB:
        bytesPerSecond /= 1024.0 * 1024.0;
        break;
    case Unit::GB:
        bytesPerSecond /= 1024.0 * 1024.0 * 1024.0;
        break;
    case Unit::TB:
        bytesPerSecond /= 1024.0 * 1024.0 * 1024.0 * 1024.0;
        break;
    }

    QString unitString;
    switch(unit)
    {
    case Unit::B:  unitString = "B/s";
        break;
    case Unit::KB: unitString = "KB/s";
        break;
    case Unit::MB: unitString = "MB/s";
        break;
    case Unit::GB: unitString = "GB/s";
        break;
    case Unit::TB: unitString = "TB/s";
        break;
    }
    return QString("%1 %2").arg(bytesPerSecond, 0, 'f', 2).arg(unitString);
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

void NetworkConnectionComponent::updateDownloadUpload(double download, double upload)
{
    networkDownloadLabel->setText(QString("↓ %1").arg(formatSpeed(download)));
    networkUploadLabel->setText(QString("↑ %1").arg(formatSpeed(upload)));
}

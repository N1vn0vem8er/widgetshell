#include "networkservice.h"
#include <qdebug.h>
#include <qdir.h>
#include <qprocess.h>

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

QString NetworkService::defaultInterface() const
{
    return m_defaultInterface;
}

void NetworkService::updateDefaultInterface()
{
    QProcess process;
    process.start("ip", {"route", "get", "1.1.1.1"});
    if (!process.waitForFinished(1000))
        return;

    const QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    const QStringList parts = output.simplified().split(' ');

    for(int i = 0; i + 1 < parts.size(); ++i) {
        if(parts[i] == "dev")
            m_defaultInterface = parts[i + 1];
    }
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
    updateDefaultInterface();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NetworkService::timeout);
    timer->setInterval(1000);
    timer->start();
}

NetworkService::~NetworkService()
{

}

void NetworkService::checkNetworkUse()
{
    quint64 rx = readCounter(QString("/sys/class/net/%1/statistics/rx_bytes").arg(m_defaultInterface));
    quint64 tx = readCounter(QString("/sys/class/net/%1/statistics/tx_bytes").arg(m_defaultInterface));
    emit networkSpeed(rx - oldRx, tx - oldTx);
    oldRx = rx;
    oldTx = tx;
}

quint64 NetworkService::readCounter(const QString &path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        return file.readAll().trimmed().toULongLong();
    return 0;
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
    updateDefaultInterface();
    const bool wasWifi = isWifi();
    m_transportMedium = medium;

    emit transportMediumChanged(m_transportMedium);
    if(wasWifi != isWifi())
        emit wifiChanged(isWifi());
}

void NetworkService::timeout()
{
    checkNetworkUse();
}



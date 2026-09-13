#include "networkservice.h"
#include <qdebug.h>
#include <qdir.h>
#include <qprocess.h>
#include <qregularexpression.h>

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
    if(!process.waitForFinished(1000))
        return;

    const QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    const QStringList parts = output.simplified().split(' ');

    for(int i = 0; i + 1 < parts.size(); ++i) {
        if(parts[i] == "dev")
            m_defaultInterface = parts[i + 1];
    }
}

QMap<QString, QString> NetworkService::getIpAddresses()
{
    QProcess process;
    process.start("ip", {"-4", "addr", "show"});
    if(!process.waitForFinished(1000))
        return {};

    const QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QMap<QString, QString> result;
    QString currentInterface;
    const QStringList lines = output.split('\n');

    for(const QString& line : lines)
    {
        static QRegularExpression interfaceRegex(R"(^\d+:\s+([^:]+):)");
        const auto interfaceMatch = interfaceRegex.match(line);

        if(interfaceMatch.hasMatch())
        {
            currentInterface = interfaceMatch.captured(1);
            continue;
        }

        static QRegularExpression addressRegex(R"(^\s+inet\s+(\d+\.\d+\.\d+\.\d+)/\d+)");
        const auto addressMatch = addressRegex.match(line);

        if(addressMatch.hasMatch() && !currentInterface.isEmpty())
            result.insert(currentInterface, addressMatch.captured(1));
    }

    return result;
}

QString NetworkService::getGatewayAddress()
{
    QProcess process;
    process.start("ip", {"route", "show", "default"});

    if(!process.waitForFinished(1000))
        return {};

    const QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    static QRegularExpression regex(R"(default\s+via\s+(\S+))");
    const auto match = regex.match(output);

    if(match.hasMatch())
        return match.captured(1);

    return {};
}

QString NetworkService::getInterfaceMacAddress(const QString &name)
{
    QFile file(QString("/sys/class/net/%1/address").arg(name));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    return QString::fromLatin1(file.readAll()).trimmed().toUpper();
}

QString NetworkService::getInterfaceGateway(const QString &name)
{
    QProcess process;
    process.start("ip", {"route", "show", "default", "dev", name});

    if(!process.waitForFinished(1000))
        return {};

    const QString output = process.readAllStandardOutput().trimmed();
    static const QRegularExpression regex(R"(^default\s+via\s+(\S+))");
    const auto match = regex.match(output);

    if(!match.hasMatch())
        return {};

    return match.captured(1);
}

int NetworkService::getInterfaceSpeed(const QString &name)
{
    QFile file(QString("/sys/class/net/%1/speed").arg(name));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    bool ok;
    const int speed = QString::fromUtf8(file.readAll()).trimmed().toInt(&ok);
    return ok ? speed : 0;
}

QString NetworkService::getInterfaceDuplex(const QString &name)
{
    QFile file(QString("/sys/class/net/%1/duplex").arg(name));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    return QString::fromUtf8(file.readAll()).trimmed();
}

bool NetworkService::isInterfaceUp(const QString &name)
{
    QFile file(QString("/sys/class/net/%1/operstate").arg(name));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    return QString::fromUtf8(file.readAll()).trimmed() == "up";
}

bool NetworkService::isInterfaceCarrier(const QString &name)
{
    QFile file(QString("/sys/class/net/%1/carrier").arg(name));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    return file.readAll().trimmed() == "1";
}

int NetworkService::getInterfaceMtu(const QString &name)
{
    QFile file(QString("/sys/class/net/%1/mtu").arg(name));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    bool ok = false;
    const int mtu = QString::fromUtf8(file.readAll()).trimmed().toInt(&ok);

    return ok ? mtu : 0;
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



#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QNetworkInformation>
#include <QObject>
#include <qtimer.h>

struct NetworkInterface
{
    QString name;
    QString macAddress;
    QStringList ipv4Addresses;
    QStringList ipv6Addresses;
    QString gateway;
    bool up;
    bool carrier;
    int mtu;
    int speed;
    QString duplex;
    quint64 rxBytes;
    quint64 txBytes;
    quint64 rxPackets;
    quint64 txPackets;
    quint64 rxErrors;
    quint64 txErrors;
    quint64 rxDropped;
    quint64 txDropped;
    QString ssid;
    int signalDbm;
    int frequency;
    double rxBitrate;
    double txBitrate;
};

class NetworkService : public QObject
{
    Q_OBJECT
public:
    static NetworkService* instance();
    static NetworkService* create(QObject* parent = nullptr);
    static void destroy();
    NetworkService(const NetworkService&) = delete;
    NetworkService& operator=(const NetworkService&) = delete;
    bool isOnline() const;
    bool isWifi() const;
    QNetworkInformation::Reachability reachability() const;
    QNetworkInformation::TransportMedium transportMedium() const;
    QString defaultInterface() const;
    void updateDefaultInterface();
    QMap<QString, QString> getIpAddresses();
    QString getGatewayAddress();
    QString getInterfaceMacAddress(const QString& name);
    QString getInterfaceGateway(const QString& name);
    QString getInterfaceSpeed(const QString& name);

private:
    inline static NetworkService* m_instance = nullptr;
    QNetworkInformation* m_networkInfo = nullptr;
    QString m_defaultInterface;
    QNetworkInformation::Reachability m_reachability;
    QNetworkInformation::TransportMedium m_transportMedium;
    QTimer* timer;
    quint64 oldTx {0};
    quint64 oldRx {0};

private:
    explicit NetworkService(QObject *parent = nullptr);
    ~NetworkService();
    void checkNetworkUse();
    quint64 readCounter(const QString& path);

private slots:
    void updateReachability(QNetworkInformation::Reachability reachability);
    void updateTransportMedium(QNetworkInformation::TransportMedium medium);
    void timeout();

signals:
    void onlineChanged(bool online);
    void wifiChanged(bool wifi);
    void reachabilityChanged(QNetworkInformation::Reachability reachability);
    void transportMediumChanged(QNetworkInformation::TransportMedium medium);
    void networkSpeed(double download, double upload);

};

#endif // NETWORKSERVICE_H

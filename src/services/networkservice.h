#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QNetworkInformation>
#include <QObject>

class NetworkService : public QObject
{
    Q_OBJECT
public:
    static NetworkService* instance();
    static NetworkService* create(QObject* parent = nullptr);
    static void destroy();
    NetworkService(const NetworkService&) = delete;
    NetworkService& operator=(const NetworkService&) = delete;

private:
    inline static NetworkService* m_instance = nullptr;
    QNetworkInformation* m_networkInfo = nullptr;
    QNetworkInformation::Reachability m_reachability;
    QNetworkInformation::TransportMedium m_transportMedium;

private:
    explicit NetworkService(QObject *parent = nullptr);
    ~NetworkService();

private slots:
    void updateReachability(QNetworkInformation::Reachability reachability);
    void updateTransportMedium(QNetworkInformation::TransportMedium medium);

signals:
};

#endif // NETWORKSERVICE_H

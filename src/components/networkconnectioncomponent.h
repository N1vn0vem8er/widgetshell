#ifndef NETWORKCONNECTIONCOMPONENT_H
#define NETWORKCONNECTIONCOMPONENT_H

#include "menus/networkmenu.h"
#include "services/networkservice.h"
#include <QWidget>
#include <qlabel.h>
#include <qpushbutton.h>

class NetworkConnectionComponent : public QWidget
{
    Q_OBJECT
public:
    enum class Unit {
        B,
        KB,
        MB,
        GB,
        TB
    };
    explicit NetworkConnectionComponent(NetworkConnectionComponent::Unit unit, QWidget *parent = nullptr);
    ~NetworkConnectionComponent();

private:
    NetworkService* service;
    QPushButton* connectionbutton;
    QLabel* networkDownloadLabel;
    QLabel* networkUploadLabel;
    QNetworkInformation::TransportMedium m_transportMedium;
    QNetworkInformation::Reachability m_reachability;
    Unit unit {Unit::MB};
    NetworkMenu* networkMenu {nullptr};

private:
    void updateIcons();
    QString createTooltipText(QNetworkInformation::TransportMedium transportMedium, QNetworkInformation::Reachability reachability) const;
    QString formatSpeed(double bytesPerSecond) const;

private slots:
    void updateTransportMedium(QNetworkInformation::TransportMedium transportMedium);
    void updateReachability(QNetworkInformation::Reachability reachability);
    void updateDownloadUpload(double download, double upload);

signals:
};

#endif // NETWORKCONNECTIONCOMPONENT_H

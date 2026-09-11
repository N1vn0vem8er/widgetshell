#ifndef NETWORKCONNECTIONCOMPONENT_H
#define NETWORKCONNECTIONCOMPONENT_H

#include "services/networkservice.h"
#include <QWidget>
#include <qlabel.h>
#include <qpushbutton.h>

class NetworkConnectionComponent : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkConnectionComponent(QWidget *parent = nullptr);

private:
    NetworkService* service;
    QPushButton* connectionbutton;
    QNetworkInformation::TransportMedium m_transportMedium;
    QNetworkInformation::Reachability m_reachability;

private:
    void updateIcons();
    QString createTooltipText(QNetworkInformation::TransportMedium transportMedium, QNetworkInformation::Reachability reachability) const;

private slots:
    void updateTransportMedium(QNetworkInformation::TransportMedium transportMedium);
    void updateReachability(QNetworkInformation::Reachability reachability);

signals:
};

#endif // NETWORKCONNECTIONCOMPONENT_H

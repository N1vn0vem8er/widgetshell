#ifndef NETWORKCONNECTIONCOMPONENT_H
#define NETWORKCONNECTIONCOMPONENT_H

#include "services/networkservice.h"
#include <QWidget>
#include <qlabel.h>

class NetworkConnectionComponent : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkConnectionComponent(QWidget *parent = nullptr);

private:
    NetworkService* service;
    QLabel* connectionTypeLabel;

signals:
};

#endif // NETWORKCONNECTIONCOMPONENT_H

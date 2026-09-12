#ifndef NETWORKMENU_H
#define NETWORKMENU_H

#include "services/networkservice.h"
#include <LayerShellQt/window.h>
#include <QLabel>
#include <QListWidget>
#include <QWidget>

class NetworkMenu : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkMenu(QWidget *parent = nullptr);
    void showForWidget(QWidget* widget);

private:
    LayerShellQt::Window* layerWindow;
    NetworkService* service;
    QLabel* ipAddressLabel;
    QLabel* gatewayAddressLabel;
    QListWidget* ipList;

signals:
};

#endif // NETWORKMENU_H

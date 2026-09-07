#ifndef OPENEDAPPSCOMPONENT_H
#define OPENEDAPPSCOMPONENT_H

#include "services/desktopappsservice.h"
#include "services/hyprlandservice.h"
#include <QWidget>
#include <qboxlayout.h>
#include <qpushbutton.h>

class OpenedAppsComponent : public QWidget
{
    Q_OBJECT
public:
    explicit OpenedAppsComponent(QWidget *parent = nullptr);
    ~OpenedAppsComponent();

private:
    HyprlandService* hyprlandService;
    DesktopAppsService* desktopAppService;
    QHBoxLayout* mainLayout;
    QList<QPushButton*> buttons;

private slots:
    void updateClients();

signals:
};

#endif // OPENEDAPPSCOMPONENT_H

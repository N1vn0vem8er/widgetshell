#ifndef OPENEDAPPSSWAYCOMPONENT_H
#define OPENEDAPPSSWAYCOMPONENT_H

#include "services/desktopappsservice.h"
#include "services/swayservice.h"
#include "components/bar/openedappscomponent.h"
#include <QWidget>
#include <qboxlayout.h>
#include <qpushbutton.h>

class OpenedAppsSwayComponent : public OpenedAppsComponent
{
    Q_OBJECT
public:
    explicit OpenedAppsSwayComponent(QWidget *parent = nullptr);

private:
    SwayService* swayService;
    DesktopAppsService* desktopAppService;
    QHBoxLayout* mainLayout;
    QList<QPushButton*> buttons;

private slots:
    void updateClients();
    void activateClient(int id);

signals:
};

#endif // OPENEDAPPSSWAYCOMPONENT_H

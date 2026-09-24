#ifndef OPENEDAPPSHYPRLANDCOMPONENT_H
#define OPENEDAPPSHYPRLANDCOMPONENT_H

#include "services/desktopappsservice.h"
#include "services/hyprlandservice.h"
#include <QWidget>
#include <qboxlayout.h>
#include <qpushbutton.h>

class OpenedAppsHyprlandComponent : public QWidget
{
    Q_OBJECT
public:
    explicit OpenedAppsHyprlandComponent(QWidget *parent = nullptr);
    ~OpenedAppsHyprlandComponent();

private:
    HyprlandService* hyprlandService;
    DesktopAppsService* desktopAppService;
    QHBoxLayout* mainLayout;
    QList<QPushButton*> buttons;

private slots:
    void updateClients();
    void activateClient(quint64 address);
    void activeEmpty();

signals:
};

#endif // OPENEDAPPSHYPRLANDCOMPONENT_H

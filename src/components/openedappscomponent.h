#ifndef OPENEDAPPSCOMPONENT_H
#define OPENEDAPPSCOMPONENT_H

#include "services/hyprlandservice.h"
#include <QWidget>

class OpenedAppsComponent : public QWidget
{
    Q_OBJECT
public:
    explicit OpenedAppsComponent(QWidget *parent = nullptr);
    ~OpenedAppsComponent();

private:
    HyprlandService* hyprlandService;

private slots:
    void updateClients();

signals:
};

#endif // OPENEDAPPSCOMPONENT_H

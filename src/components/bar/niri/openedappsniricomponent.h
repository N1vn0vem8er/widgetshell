#ifndef OPENEDAPPSNIRICOMPONENT_H
#define OPENEDAPPSNIRICOMPONENT_H

#include "components/bar/openedappscomponent.h"
#include "services/desktopappsservice.h"
#include "services/niriservice.h"
#include <qboxlayout.h>
#include <qpushbutton.h>

class OpenedAppsNiriComponent : public OpenedAppsComponent
{
    Q_OBJECT
public:
    OpenedAppsNiriComponent(QWidget* parent = nullptr);

private:
    NiriService* niriService;
    DesktopAppsService* desktopAppService;
    QHBoxLayout* mainLayout;
    QList<QPushButton*> buttons;

private slots:
    void updateClients();
    void activateClient(quint64 id);
};

#endif // OPENEDAPPSNIRICOMPONENT_H

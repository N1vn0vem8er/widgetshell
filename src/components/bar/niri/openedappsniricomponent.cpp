#include "openedappsniricomponent.h"

OpenedAppsNiriComponent::OpenedAppsNiriComponent(QWidget *parent) : OpenedAppsComponent{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    niriService = NiriService::instance();
    desktopAppService = DesktopAppsService::instance();

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
    setLayout(mainLayout);
    updateClients();
}

void OpenedAppsNiriComponent::updateClients()
{

}

void OpenedAppsNiriComponent::activateClient(quint64 id)
{

}

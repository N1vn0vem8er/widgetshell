#include "openedappscomponent.h"

OpenedAppsComponent::OpenedAppsComponent(QWidget *parent)
    : QWidget{parent}
{
    hyprlandService = HyprlandService::instance();
    desktopAppService = DesktopAppsService::instance();
}

OpenedAppsComponent::~OpenedAppsComponent()
{

}

void OpenedAppsComponent::updateClients()
{

}

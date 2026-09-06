#include "openedappscomponent.h"

OpenedAppsComponent::OpenedAppsComponent(QWidget *parent)
    : QWidget{parent}
{
    hyprlandService = HyprlandService::instance();
}

OpenedAppsComponent::~OpenedAppsComponent()
{

}

void OpenedAppsComponent::updateClients()
{

}

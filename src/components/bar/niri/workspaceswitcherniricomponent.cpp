#include "workspaceswitcherniricomponent.h"

WorkspaceSwitcherNiriComponent::WorkspaceSwitcherNiriComponent(QWidget *parent) : WorkspaceSwitcherComponent(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
    setLayout(mainLayout);

    service = NiriService::instance();
    connect(service, &NiriService::workspaceActivated, this, &WorkspaceSwitcherNiriComponent::workspaceChanged);
    refreshWorkspaces();
    service->refreshWorkspaces();
}

void WorkspaceSwitcherNiriComponent::refreshWorkspaces()
{
}

void WorkspaceSwitcherNiriComponent::workspaceChanged(quint64 id)
{

}

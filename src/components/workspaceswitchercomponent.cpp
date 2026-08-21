#include "workspaceswitchercomponent.h"

WorkspaceSwitcherComponent::WorkspaceSwitcherComponent(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
    setLayout(mainLayout);

    service = HyprlandService::instance();

    if(service)
    {
        connect(service, &HyprlandService::workspacesChanged, this, &WorkspaceSwitcherComponent::refreshWorkspaces);
    }
    refreshWorkspaces();
}

void WorkspaceSwitcherComponent::refreshWorkspaces()
{
    QList<HyprlandWorkspace> workspaces = service->getWorkspaces();
    const int neededButtons = workspaces.size();

    while(buttons.size() < neededButtons)
    {
        QPushButton* button = new QPushButton(this);
        mainLayout->addWidget(button);
        buttons.append(button);
    }

    for(int i = 0; i < buttons.size(); i++)
    {
        if(i < neededButtons)
        {
            const auto& ws = workspaces.at(i);
            QPushButton* button = buttons.at(i);
            button->setText(QString::number(ws.id));
            button->setProperty("workspaceId", ws.id);
            button->show();
        }
        else
            buttons.at(i)->hide();
    }
}

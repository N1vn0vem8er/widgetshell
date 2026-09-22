#include "workspaceswitcherswaycomponent.h"

WorkspaceSwitcherSwayComponent::WorkspaceSwitcherSwayComponent(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
    setLayout(mainLayout);

    service = SwayService::instance();

    if(service)
    {
        connect(service, &SwayService::workspacesChanged, this, &WorkspaceSwitcherSwayComponent::workspacesChanged);
    }
}

void WorkspaceSwitcherSwayComponent::workspacesChanged()
{
    if(!service)
        return;
    QList<SwayWorkspace> workspaces = service->workspaces();
    const int neededButtons = workspaces.size();

    while(buttons.size() < neededButtons)
    {
        WorkspaceButton* button = new WorkspaceButton(this);
        button->setCheckable(true);
        mainLayout->addWidget(button);
        buttons.append(button);
    }

    for(int i = 0; i < buttons.size(); i++)
    {
        if(i < neededButtons)
        {
            const auto& ws = workspaces.at(i);
            WorkspaceButton* button = buttons.at(i);
            button->setText(ws.name);
            button->setProperty("workspaceId", ws.id);
            // if(ws.id == currentWorkspace)
            //     button->setChecked(true);
            connect(button, &WorkspaceButton::clicked, this, [button, this]{
                int workspaceId = button->property("workspaceId").toInt();
                // if(workspaceId == currentWorkspace)
                //     button->setChecked(true);
                // else
                //     service->switchWorkspace(workspaceId);
            });
            button->show();
        }
        else
            buttons.at(i)->hide();
    }
}

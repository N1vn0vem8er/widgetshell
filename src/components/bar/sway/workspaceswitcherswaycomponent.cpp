#include "workspaceswitcherswaycomponent.h"

WorkspaceSwitcherSwayComponent::WorkspaceSwitcherSwayComponent(QWidget *parent)
    : WorkspaceSwitcherComponent{parent}
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
            button->setProperty("workspaceName", ws.name);
            if(ws.focused)
                button->setChecked(true);
            else
                button->setChecked(false);
            connect(button, &WorkspaceButton::clicked, this, [button, this]{
                auto workspaceName = button->property("workspaceName").toString();
                service->focusWorkspace(workspaceName);
            });
            button->show();
        }
        else
            buttons.at(i)->hide();
    }
}

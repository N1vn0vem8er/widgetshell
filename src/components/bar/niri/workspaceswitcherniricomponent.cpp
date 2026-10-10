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
    connect(service, &NiriService::workspacesRefreshed, this, &WorkspaceSwitcherNiriComponent::refreshWorkspaces);
    service->refreshWorkspaces();
}

void WorkspaceSwitcherNiriComponent::refreshWorkspaces()
{
    if(!service)
        return;
    QList<NiriWorkspace> workspaces = service->workspaces();
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
            button->setText(QString::number(ws.idx));
            button->setProperty("id", QString::number(ws.id));
            if(ws.isFocused)
                button->setChecked(true);
            else
                button->setChecked(false);
            connect(button, &WorkspaceButton::clicked, this, [button, this]{
                auto workspaceId = button->property("id").toULongLong();
                service->focusWorkspace(workspaceId);
            });
            button->show();
        }
        else
            buttons.at(i)->hide();
    }
}

void WorkspaceSwitcherNiriComponent::workspaceChanged(quint64 id)
{
    for(auto* button : std::as_const(buttons))
    {
        if(button->property("id").toULongLong() == id)
            button->setChecked(true);
        else
            button->setChecked(false);
    }
}

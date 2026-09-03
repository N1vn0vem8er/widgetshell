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
        connect(service, &HyprlandService::workspaceChanged, this, &WorkspaceSwitcherComponent::workspaceChanged);
    }
    refreshWorkspaces();
}

void WorkspaceSwitcherComponent::refreshWorkspaces()
{
    if(!service)
        return;
    QList<HyprlandWorkspace> workspaces = service->getWorkspaces();
    const int neededButtons = workspaces.size();

    while(buttons.size() < neededButtons)
    {
        QPushButton* button = new QPushButton(this);
        button->setCheckable(true);
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
            connect(button, &QPushButton::clicked, this, [button, this]{
                service->switchWorkspace(button->property("workspaceId").toInt());
            });
            button->show();
        }
        else
            buttons.at(i)->hide();
    }
}

void WorkspaceSwitcherComponent::workspaceChanged(int id)
{
    for(auto* button : std::as_const(buttons))
    {
        if(button->property("workspaceId").toInt() == id)
            button->setChecked(true);
        else
            button->setChecked(false);
    }
}

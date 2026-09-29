#ifndef WORKSPACESWITCHERHYPRLANDCOMPONENT_H
#define WORKSPACESWITCHERHYPRLANDCOMPONENT_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "components/bar/workspacebutton.h"
#include "services/hyprlandservice.h"
#include "components/bar/workspaceswitchercomponent.h"

class WorkspaceSwitcherHyprlandComponent : public WorkspaceSwitcherComponent
{
    Q_OBJECT
public:
    explicit WorkspaceSwitcherHyprlandComponent(QWidget *parent = nullptr);

private:
    QHBoxLayout* mainLayout;
    QList<WorkspaceButton*> buttons;
    HyprlandService* service;
    int currentWorkspace;

private slots:
    void refreshWorkspaces();
    void workspaceChanged(int id);

signals:
};

#endif // WORKSPACESWITCHERHYPRLANDCOMPONENT_H

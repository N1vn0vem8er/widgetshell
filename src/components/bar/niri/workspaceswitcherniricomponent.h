#ifndef WORKSPACESWITCHERNIRICOMPONENT_H
#define WORKSPACESWITCHERNIRICOMPONENT_H

#include "components/bar/workspacebutton.h"
#include "components/bar/workspaceswitchercomponent.h"
#include "services/niriservice.h"
#include <qboxlayout.h>

class WorkspaceSwitcherNiriComponent : public WorkspaceSwitcherComponent
{
    Q_OBJECT
public:
    WorkspaceSwitcherNiriComponent(QWidget* parent = nullptr);

private:
    QHBoxLayout* mainLayout;
    QList<WorkspaceButton*> buttons;
    NiriService* service {nullptr};

private slots:
    void refreshWorkspaces();
    void workspaceChanged(quint64 id);
};

#endif // WORKSPACESWITCHERNIRICOMPONENT_H

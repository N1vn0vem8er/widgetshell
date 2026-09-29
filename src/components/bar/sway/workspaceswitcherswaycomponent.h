#ifndef WORKSPACESWITCHERSWAYCOMPONENT_H
#define WORKSPACESWITCHERSWAYCOMPONENT_H

#include "components/bar/workspacebutton.h"
#include "components/bar/workspaceswitchercomponent.h"
#include "services/swayservice.h"
#include <QWidget>
#include <qboxlayout.h>

class WorkspaceSwitcherSwayComponent : public WorkspaceSwitcherComponent
{
    Q_OBJECT
public:
    explicit WorkspaceSwitcherSwayComponent(QWidget *parent = nullptr);

private:
    QHBoxLayout* mainLayout;
    QList<WorkspaceButton*> buttons;
    SwayService* service;

private slots:
    void workspacesChanged();

signals:
};

#endif // WORKSPACESWITCHERSWAYCOMPONENT_H

#ifndef WORKSPACESWITCHERCOMPONENT_H
#define WORKSPACESWITCHERCOMPONENT_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "components/workspacebutton.h"
#include "services/hyprlandservice.h"

class WorkspaceSwitcherComponent : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceSwitcherComponent(QWidget *parent = nullptr);

private:
    QHBoxLayout* mainLayout;
    QList<WorkspaceButton*> buttons;
    HyprlandService* service;

private slots:
    void refreshWorkspaces();
    void workspaceChanged(int id);

signals:
};

#endif // WORKSPACESWITCHERCOMPONENT_H

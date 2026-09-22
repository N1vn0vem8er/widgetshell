#ifndef WORKSPACESWITCHERSWAYCOMPONENT_H
#define WORKSPACESWITCHERSWAYCOMPONENT_H

#include "components/bar/workspacebutton.h"
#include "services/swayservice.h"
#include <QWidget>
#include <qboxlayout.h>

class WorkspaceSwitcherSwayComponent : public QWidget
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

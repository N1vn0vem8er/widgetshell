#ifndef WORKSPACESWITCHERCOMPONENT_H
#define WORKSPACESWITCHERCOMPONENT_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "services/hyprlandservice.h"

class WorkspaceSwitcherComponent : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceSwitcherComponent(QWidget *parent = nullptr);

private:
    QHBoxLayout* mainLayout;
    QList<QPushButton*> buttons;
    HyprlandService* service;

private slots:
    void refreshWorkspaces();

signals:
};

#endif // WORKSPACESWITCHERCOMPONENT_H

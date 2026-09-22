#ifndef BARWIDGET_H
#define BARWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QWidget>

#include "components/bar/hyprland/openedappscomponent.h"
#include "components/bar/hyprland/workspaceswitchercomponent.h"
#include "menus/controlmenu.h"

class BarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BarWidget(QWidget *parent = nullptr);
    ~BarWidget();

private:
    QGridLayout* mainLayout;
    QLabel* dateTimeLabel;
    QTimer* dateTimeTimer;
    QPushButton* controlMenuButton;
    WorkspaceSwitcherComponent* workspaceSwitcher;
    ControlMenu* controlMenu {nullptr};
    OpenedAppsComponent* openedApps;

private:
    void createBar();

signals:
};

#endif // BARWIDGET_H

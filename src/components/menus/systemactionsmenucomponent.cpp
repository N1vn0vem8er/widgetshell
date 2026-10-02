#include "systemactionsmenucomponent.h"
#include <QProcess>
#include <qapplication.h>
#include <qboxlayout.h>

SystemActionsMenuComponent::SystemActionsMenuComponent(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout* topButtonsLayout = new QHBoxLayout(this);
    shutdownButton = new QPushButton(QIcon::fromTheme("system-shutdown"), "", this);
    shutdownButton->setToolTip("Shutdown");
    logoutButton = new QPushButton(QIcon::fromTheme("system-log-out"), "", this);
    logoutButton->setToolTip("Logout");
    sleepButton = new QPushButton(QIcon::fromTheme("system-suspend"), "", this);
    sleepButton->setToolTip("Sleep");
    rebootButton = new QPushButton(QIcon::fromTheme("system-reboot"), "", this);
    rebootButton->setToolTip("Reboot");
    connect(shutdownButton, &QPushButton::clicked, this, &SystemActionsMenuComponent::onShutdown);
    connect(logoutButton, &QPushButton::clicked, this, &SystemActionsMenuComponent::onLogout);
    connect(sleepButton, &QPushButton::clicked, this, &SystemActionsMenuComponent::onSleep);
    connect(rebootButton, &QPushButton::clicked, this, &SystemActionsMenuComponent::onReboot);
    topButtonsLayout->addWidget(sleepButton);
    topButtonsLayout->addWidget(logoutButton);
    topButtonsLayout->addWidget(rebootButton);
    topButtonsLayout->addWidget(shutdownButton);
}

void SystemActionsMenuComponent::onShutdown()
{
    QProcess::startDetached("systemctl", {"poweroff"});
}

void SystemActionsMenuComponent::onLogout()
{
    if(getenv("HYPRLAND_INSTANCE_SIGNATURE") != NULL)
    {
        QProcess::startDetached("hyprctl", {"dispatch", "exit"});
    }
    else if(getenv("SWAYSOCK") != NULL)
    {
        QProcess::startDetached("swaymsg", {"exit"});
    }
    else
        QProcess::startDetached("loginctl", {"terminate-session", getenv("XDG_SESSION_ID")});
}

void SystemActionsMenuComponent::onSleep()
{
    QProcess::startDetached("systemctl", {"suspend"});
}

void SystemActionsMenuComponent::onReboot()
{
    QProcess::startDetached("systemctl", {"reboot"});
}

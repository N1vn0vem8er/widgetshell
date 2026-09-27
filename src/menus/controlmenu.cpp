#include "controlmenu.h"

#include <LayerShellQt/Window>

#include <QProcess>

ControlMenu::ControlMenu(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedSize(300, 600);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight));

    mainLayout = new QVBoxLayout(this);

    QWidget* topButtonsWidget = new QWidget(this);
    QHBoxLayout* topButtonsLayout = new QHBoxLayout(topButtonsWidget);
    shutdownButton = new QPushButton(QIcon::fromTheme("system-shutdown"), "", this);
    shutdownButton->setToolTip("Shutdown");
    logoutButton = new QPushButton(QIcon::fromTheme("system-log-out"), "", this);
    logoutButton->setToolTip("Logout");
    sleepButton = new QPushButton(QIcon::fromTheme("system-suspend"), "", this);
    sleepButton->setToolTip("Sleep");
    rebootButton = new QPushButton(QIcon::fromTheme("system-reboot"), "", this);
    rebootButton->setToolTip("Reboot");
    connect(shutdownButton, &QPushButton::clicked, this, &ControlMenu::onShutdown);
    connect(logoutButton, &QPushButton::clicked, this, &ControlMenu::onLogout);
    connect(sleepButton, &QPushButton::clicked, this, &ControlMenu::onSleep);
    connect(rebootButton, &QPushButton::clicked, this, &ControlMenu::onReboot);
    topButtonsLayout->addWidget(sleepButton);
    topButtonsLayout->addWidget(logoutButton);
    topButtonsLayout->addWidget(rebootButton);
    topButtonsLayout->addWidget(shutdownButton);
    mainLayout->addWidget(topButtonsWidget);

    audioVolumeControl = new AudioVolumeControlComponent(this);
    mainLayout->addWidget(audioVolumeControl);

    calendar = new QCalendarWidget(this);
    mainLayout->addWidget(calendar);

    notificationHistory = new NotificationHistoryComponent(this);
    mainLayout->addWidget(notificationHistory);

    setLayout(mainLayout);
}

void ControlMenu::onShutdown()
{
    QProcess::startDetached("systemctl", {"poweroff"});
}

void ControlMenu::onLogout()
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

void ControlMenu::onSleep()
{
    QProcess::startDetached("systemctl", {"suspend"});
}

void ControlMenu::onReboot()
{
    QProcess::startDetached("systemctl", {"reboot"});
}

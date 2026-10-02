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

    audioVolumeControl = new AudioVolumeControlComponent(this);
    mainLayout->addWidget(audioVolumeControl);

    calendar = new QCalendarWidget(this);
    mainLayout->addWidget(calendar);

    notificationHistory = new NotificationHistoryComponent(this);
    mainLayout->addWidget(notificationHistory);

    setLayout(mainLayout);
}

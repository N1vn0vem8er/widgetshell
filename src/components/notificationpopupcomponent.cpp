#include "notificationpopupcomponent.h"
#include <LayerShellQt/window.h>
#include <qboxlayout.h>

NotificationPopupComponent::NotificationPopupComponent(const Notification &notification, QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(200, 50);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerTop);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight));

    body = new QLabel(this);
    body->setText(notification.body);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(body);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]{hide(); deleteLater();});
    timer->setInterval(notification.expireTimeout <= 0 ? 5000 : notification.expireTimeout);
    timer->setSingleShot(true);
    timer->start();
}
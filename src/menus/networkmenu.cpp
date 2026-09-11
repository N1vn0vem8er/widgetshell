#include "networkmenu.h"
#include <LayerShellQt/window.h>

NetworkMenu::NetworkMenu(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 600);
    winId();
    layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft));
}

void NetworkMenu::showForWidget(QWidget* widget)
{
    QPoint position = widget->mapToGlobal(QPoint(0, widget->height()));
    layerWindow->setMargins(QMargins(position.x(), 0, 0, position.y()));
    show();
}

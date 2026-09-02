#include "controlmenu.h"

#include <LayerShellQt/Window>

ControlMenu::ControlMenu(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 600);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight));
}

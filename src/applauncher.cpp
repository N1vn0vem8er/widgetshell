#include "applauncher.h"
#include <LayerShellQt/window.h>

AppLauncher::AppLauncher(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 500);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft | LayerShellQt::Window::AnchorRight | LayerShellQt::Window::AnchorBottom));
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);

}

void AppLauncher::onActivate()
{
    if(isVisible())
        hide();
    else
        show();
}

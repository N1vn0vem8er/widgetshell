#include "applauncher.h"
#include <LayerShellQt/window.h>

AppLauncher::AppLauncher(QWidget *parent)
    : QWidget{parent}
{
    ipcServerService = IPCServerService::instance();
    setFixedSize(300, 500);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorNone));
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);
    connect(ipcServerService, &IPCServerService::showLauncher, this, &AppLauncher::onActivate);
}

void AppLauncher::onActivate()
{
    if(isVisible())
        hide();
    else
        show();
}

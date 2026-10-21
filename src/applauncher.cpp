#include "applauncher.h"
#include <LayerShellQt/window.h>
#include <qboxlayout.h>

AppLauncher::AppLauncher(QWidget *parent)
    : QWidget{parent}
{
    ipcServerService = IPCServerService::instance();
    appService = DesktopAppsService::instance();
    setFixedSize(300, 500);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorNone));
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);
    connect(ipcServerService, &IPCServerService::showLauncher, this, &AppLauncher::onActivate);

    auto layout = new QVBoxLayout(this);
    searchLineEdit = new QLineEdit(this);
    layout->addWidget(searchLineEdit);
    listView = new QListView(this);
    layout->addWidget(listView);
}

void AppLauncher::refresh()
{

}

void AppLauncher::onActivate()
{
    if(isVisible())
        hide();
    else
        show();
}

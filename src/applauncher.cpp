#include "applauncher.h"
#include <LayerShellQt/window.h>
#include <QProcess>
#include <qboxlayout.h>
#include <qsettings.h>

AppLauncher::AppLauncher(QWidget *parent)
    : QWidget{parent}
{
    ipcServerService = IPCServerService::instance();
    appService = DesktopAppsService::instance();
    QSettings settings;
    setFixedSize(settings.value("AppLauncher/width", 300).toInt(), settings.value("AppLauncher/height", 500).toInt());
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
    model = new DesktopAppModel(this);
    proxyModel = new DesktopAppProxyModel(this);
    proxyModel->setSourceModel(model);
    listView->setModel(proxyModel);
    listView->setViewMode(QListView::ListMode);
    listView->setUniformItemSizes(true);
    listView->setIconSize(QSize(32, 32));
    connect(searchLineEdit, &QLineEdit::textEdited, proxyModel, &DesktopAppProxyModel::setSearchText);
    connect(listView, &QListView::activated, this, &AppLauncher::onLaunch);

    layout->addWidget(listView);

    auto closeAction = new QAction(this);
    closeAction->setShortcut(Qt::Key_Escape);
    connect(closeAction, &QAction::triggered, this, &AppLauncher::hide);
    addAction(closeAction);

    refresh();
}

void AppLauncher::refresh()
{
    model->addApps(appService->desktopApps());
}

void AppLauncher::onActivate()
{
    if(isVisible())
        hide();
    else
        show();
}

void AppLauncher::onLaunch(const QModelIndex &index)
{
    const auto app = model->appAt(index.row());
    auto command = QProcess::splitCommand(app->exec);
    if(command.isEmpty())
        return;
    QProcess::startDetached(command.constFirst(), command.mid(1));
    hide();
}

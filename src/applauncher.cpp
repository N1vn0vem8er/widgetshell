#include "applauncher.h"
#include <LayerShellQt/window.h>
#include <QKeyEvent>
#include <QProcess>
#include <qboxlayout.h>
#include <qsettings.h>

AppLauncher::AppLauncher(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    ipcServerService = IPCServerService::instance();
    appService = DesktopAppsService::instance();
    QSettings settings;
    setFixedSize(settings.value("AppLauncher/width", 300).toInt(), settings.value("AppLauncher/height", 500).toInt());
    setAttribute(Qt::WA_TranslucentBackground, settings.value("AppLauncher/TranslucentBackground", false).toBool());
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorNone));
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityExclusive);
    connect(ipcServerService, &IPCServerService::showLauncher, this, &AppLauncher::onActivate);

    auto layout = new QVBoxLayout(this);
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Search");
    searchLineEdit->setClearButtonEnabled(settings.value("AppLauncher/clearButtonEnabled", false).toBool());
    layout->addWidget(searchLineEdit);
    listView = new AppsListView(searchLineEdit, this);
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
    {
        searchLineEdit->setFocus();
        listView->setCurrentIndex(listView->model()->index(0, 0));
        show();
    }
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

void AppLauncher::keyPressEvent(QKeyEvent *event)
{
    if(searchLineEdit->hasFocus())
    {
        if(event->key() == Qt::Key_Down && listView->model()->rowCount() > 0)
        {
            listView->setCurrentIndex(listView->model()->index(0, 0));
            listView->setFocus();
            return;
        }
        if(event->key() == Qt::Key_Up && listView->currentIndex().row() > 0)
        {
            listView->setCurrentIndex(listView->model()->index(listView->currentIndex().row() - 1, 0));
            listView->setFocus();
            return;
        }
    }
    if(listView->hasFocus() && event->key() == Qt::Key_Up && listView->currentIndex().row() == 0)
    {
        searchLineEdit->setFocus();
        searchLineEdit->setCursorPosition(searchLineEdit->text().size());
        return;
    }

    QWidget::keyPressEvent(event);
}

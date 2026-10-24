#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include "models/desktopappmodel.h"
#include "models/desktopappproxymodel.h"
#include "services/desktopappsservice.h"
#include "services/ipcserverservice.h"
#include <QLineEdit>
#include <QWidget>
#include <qlistview.h>

class AppLauncher : public QWidget
{
    Q_OBJECT
public:
    explicit AppLauncher(QWidget *parent = nullptr);

private:
    IPCServerService* ipcServerService;
    DesktopAppsService* appService;
    QLineEdit* searchLineEdit;
    QListView* listView;
    DesktopAppModel* model;
    DesktopAppProxyModel* proxyModel;

private:
    void refresh();

private slots:
    void onActivate();
    void onLaunch(const QModelIndex& index);

signals:
};

#endif // APPLAUNCHER_H

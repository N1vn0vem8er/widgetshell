#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

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

private:
    void refresh();

private slots:
    void onActivate();

signals:
};

#endif // APPLAUNCHER_H

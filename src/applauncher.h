#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include "services/ipcserverservice.h"
#include <QWidget>

class AppLauncher : public QWidget
{
    Q_OBJECT
public:
    explicit AppLauncher(QWidget *parent = nullptr);

private:
    IPCServerService* ipcServerService;

private slots:
    void onActivate();

signals:
};

#endif // APPLAUNCHER_H

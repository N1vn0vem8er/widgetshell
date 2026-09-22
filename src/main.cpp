#include "barwidget.h"
#include "components/wallpaper.h"
#include "controllers/notificationpopupscontroller.h"
#include "services/hyprlandservice.h"
#include "services/desktopappsservice.h"
#include "services/systemmonitorservice.h"
#include "services/networkservice.h"
#include "services/notificationservice.h"
#include "services/mprisservice.h"
#include "services/swayservice.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("widgetshell");
    QCoreApplication::setApplicationName("widgetshell");

    QFile file(":/themes/default.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(file.readAll());
        file.close();
    }

    HyprlandService::create();
    SwayService::create();
    AudioService::create();
    DesktopAppsService::create();
    SystemMonitorService::create();
    NetworkService::create();
    NotificationService::create();
    MprisService::create();
    NotificationPopupsController notifications;
    BarWidget bar;
    bar.show();
    Wallpaper wallpaper;
    wallpaper.show();
    wallpaper.initWallpaper();
    return QApplication::exec();
}

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
#include "services/niriservice.h"

#include <QApplication>
#include <QFile>
#include <qdir.h>
#include <qfileinfo.h>
#include <qsettings.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("widgetshell");
    QCoreApplication::setApplicationName("widgetshell");
    QSettings settings;
    const QString theme = settings.value("theme", "default").toString();
    if(theme != "system")
    {
        const QString path = theme.isEmpty() || theme == "default" ? ":/themes/default.qss" : QFileInfo(settings.fileName()).dir().absolutePath() + "/themes/" + theme;
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            a.setStyleSheet(file.readAll());
            file.close();
        }
    }

    if(getenv("HYPRLAND_INSTANCE_SIGNATURE") != NULL)
        HyprlandService::create(&a);
    else if(getenv("SWAYSOCK") != NULL)
        SwayService::create(&a);
    NiriService::create(&a);
    AudioService::create(&a);
    DesktopAppsService::create(&a);
    SystemMonitorService::create(&a);
    NetworkService::create(&a);
    NotificationService::create(&a);
    MprisService::create(&a);
    NotificationPopupsController notifications;
    BarWidget bar;
    bar.show();
    Wallpaper wallpaper;
    wallpaper.show();
    wallpaper.initWallpaper();
    return a.exec();
}

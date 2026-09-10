#include "barwidget.h"
#include "components/wallpaper.h"
#include "services/hyprlandservice.h"
#include "services/desktopappsservice.h"
#include "services/systemmonitorservice.h"
#include "services/networkservice.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/themes/default.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(file.readAll());
        file.close();
    }

    HyprlandService::create();
    AudioService::create();
    DesktopAppsService::create();
    SystemMonitorService::create();
    NetworkService::create();
    BarWidget bar;
    bar.show();
    Wallpaper wallpaper;
    wallpaper.show();
    wallpaper.initWallpaper();
    return QApplication::exec();
}

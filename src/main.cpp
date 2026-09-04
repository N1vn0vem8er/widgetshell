#include "barwidget.h"
#include "components/wallpaper.h"
#include "services/hyprlandservice.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HyprlandService::create();
    AudioService::create();
    BarWidget bar;
    bar.show();
    Wallpaper wallpaper;
    wallpaper.show();
    wallpaper.initWallpaper();
    return QApplication::exec();
}

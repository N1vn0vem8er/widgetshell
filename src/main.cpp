#include "barwidget.h"
#include "services/hyprlandservice.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HyprlandService::create();
    BarWidget bar;
    bar.show();
    return QApplication::exec();
}

#include "barwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BarWidget bar;
    bar.show();
    return QApplication::exec();
}

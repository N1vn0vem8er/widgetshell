#include "appslistview.h"

#include <QKeyEvent>
#include <qapplication.h>

AppsListView::AppsListView(QLineEdit* lineEdit, QWidget *parent) : lineEdit(lineEdit), QListView(parent)
{}

void AppsListView::keyPressEvent(QKeyEvent *event)
{
    if(!event->text().isEmpty())
    {
        lineEdit->setFocus();
        QApplication::sendEvent(lineEdit, event);
        return;
    }
    QListView::keyPressEvent(event);
}

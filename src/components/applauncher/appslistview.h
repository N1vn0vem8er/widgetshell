#ifndef APPSLISTVIEW_H
#define APPSLISTVIEW_H

#include <QListView>
#include <QLineEdit>

class AppsListView : public QListView
{
    Q_OBJECT
public:
    AppsListView(QLineEdit* lineEdit, QWidget* parent = nullptr);

private:
    QLineEdit* lineEdit;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // APPSLISTVIEW_H

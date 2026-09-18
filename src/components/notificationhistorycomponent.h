#ifndef NOTIFICATIONHISTORYCOMPONENT_H
#define NOTIFICATIONHISTORYCOMPONENT_H

#include <QLineEdit>
#include <QWidget>
#include <qboxlayout.h>
#include <qlistwidget.h>
#include <qpushbutton.h>

class NotificationHistoryComponent : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationHistoryComponent(QWidget *parent = nullptr);

private:
    QVBoxLayout* mainLayout;
    QLineEdit* searchLineEdit;
    QPushButton* clearButton;
    QListWidget* listWidget;

signals:
};

#endif // NOTIFICATIONHISTORYCOMPONENT_H

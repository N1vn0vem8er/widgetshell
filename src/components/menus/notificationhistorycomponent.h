#ifndef NOTIFICATIONHISTORYCOMPONENT_H
#define NOTIFICATIONHISTORYCOMPONENT_H

#include "services/notificationservice.h"
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
    NotificationService* service;

private slots:
    void addNotification(const Notification& notification);
    void clearNotifications();
    void searchTextChanged(const QString& text);

signals:
};

#endif // NOTIFICATIONHISTORYCOMPONENT_H

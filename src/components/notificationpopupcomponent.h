#ifndef NOTIFICATIONPOPUPCOMPONENT_H
#define NOTIFICATIONPOPUPCOMPONENT_H

#include "services/notificationservice.h"
#include <QWidget>
#include <qlabel.h>
#include <qtimer.h>

class NotificationPopupComponent : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationPopupComponent(const Notification &notification, QWidget *parent = nullptr);

private:
    QLabel* body;
    QTimer* timer;

signals:
    void closed();
};

#endif // NOTIFICATIONPOPUPCOMPONENT_H

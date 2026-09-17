#ifndef NOTIFICATIONPOPUPSCONTROLLER_H
#define NOTIFICATIONPOPUPSCONTROLLER_H

#include "components/notificationpopupcomponent.h"
#include "services/notificationservice.h"
#include <QObject>

class NotificationPopupsController : public QObject
{
    Q_OBJECT
public:
    explicit NotificationPopupsController(QObject *parent = nullptr);

public slots:
    void showNotification(const Notification& notification);

private:
    void repositionPopups();

private:
    QList<NotificationPopupComponent*> m_popups;

signals:
};

#endif // NOTIFICATIONPOPUPSCONTROLLER_H

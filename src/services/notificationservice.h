#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include <QWidget>

class NotificationService : public QObject
{
    Q_OBJECT
public:
    static NotificationService* instance();
    static NotificationService* create(QObject* parent = nullptr);
    static void destroy();
    NotificationService(const NotificationService&) = delete;
    NotificationService& operator=(const NotificationService&) = delete;

private:
    explicit NotificationService(QObject *parent = nullptr);
    ~NotificationService();

private:
    inline static NotificationService* m_instance {nullptr};

signals:
};

#endif // NOTIFICATIONSERVICE_H

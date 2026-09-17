#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include <QWidget>

struct Notification {
    uint id;
    QString appName;
    QString appIcon;
    QString summary;
    QString body;
    QStringList actions;
    QVariantMap hints;
    int expireTimeout;
};

class NotificationService : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Notifications")
public:
    static NotificationService* instance();
    static NotificationService* create(QObject* parent = nullptr);
    static void destroy();
    NotificationService(const NotificationService&) = delete;
    NotificationService& operator=(const NotificationService&) = delete;
    void closeNotification(uint id);

private:
    explicit NotificationService(QObject *parent = nullptr);
    ~NotificationService();

private:
    inline static NotificationService* m_instance {nullptr};
    uint m_nextId = 1;

public slots:
    uint Notify(const QString& appName, uint replacesId, const QString& appIcon, const QString& summary, const QString& body, const QStringList& actions, const QVariantMap& hints, int expireTimeout);
    QStringList GetCapabilities();
    void GetServerInformation(QString& name, QString& vendor, QString& version, QString& specVersion);

signals:
    // void notificationReceived(uint id, const QString& appName, const QString& appIcon, const QString& summary, const QString& body, const QStringList& actions, const QVariantMap& hints, int expireTimeout);
    void notificationReceived(const Notification& notification);
    void notificationClosed(uint id, uint reason);
    void actionInvoked(uint id, const QString& action);
};

#endif // NOTIFICATIONSERVICE_H

#include "notificationservice.h"

#include <QDBusConnection>
#include <QDBusError>
#include <QDBusMessage>

NotificationService *NotificationService::instance()
{
    return m_instance;
}

NotificationService *NotificationService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new NotificationService(parent);
    return m_instance;
}

void NotificationService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

void NotificationService::closeNotification(uint id)
{
    emit notificationClosed(id, 3);
    auto dbus = QDBusConnection::sessionBus();
    dbus.send(QDBusMessage::createSignal("/org/freedesktop/Notifications", "org.freedesktop.Notifications", "NotificationClosed").operator<<(QVariant::fromValue(id)).operator<<(QVariant::fromValue<uint>(3)));
}

NotificationService::NotificationService(QObject *parent)
    : QObject{parent}
{
    auto dbus = QDBusConnection::sessionBus();
    if(!dbus.isConnected())
    {
        qWarning() << "Cannot connect to D-Bus session bus:" << dbus.lastError().message();
        return;
    }
    if(!dbus.registerService("org.freedesktop.Notifications"))
    {
        qWarning() << "Failed to register org.freedesktop.Notifications:" << dbus.lastError().message();
        return;
    }
    if(!dbus.registerObject("/org/freedesktop/Notifications", this, QDBusConnection::ExportAllSlots))
    {
        qWarning() << "Failed to register notification object:" << dbus.lastError().message();
        dbus.unregisterService("org.freedesktop.Notifications");
        return;
    }
}

NotificationService::~NotificationService()
{
    auto dbus = QDBusConnection::sessionBus();
    dbus.unregisterObject("/org/freedesktop/Notifications");
    dbus.unregisterService("org.freedesktop.Notifications");
}

QList<Notification> NotificationService::notificationHistory() const
{
    return m_notificationHistory;
}

void NotificationService::clearNotificationHistory()
{
    m_notificationHistory.clear();
}

uint NotificationService::Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int expireTimeout)
{
    uint id = replacesId;
    if(id == 0)
        id = m_nextId++;

    Notification notification;
    notification.id = id;
    notification.appName = appName;
    notification.appIcon = appIcon;
    notification.summary = summary;
    notification.body = body;
    notification.actions = actions;
    notification.hints = hints;
    notification.expireTimeout = expireTimeout;

    m_notificationHistory.append(notification);
    emit notificationReceived(notification);
    return id;
}

QStringList NotificationService::GetCapabilities()
{
    return {"actions", "body", "body-markup", "icon-static"};
}

void NotificationService::GetServerInformation(QString &name, QString &vendor, QString &version, QString &specVersion)
{
    name = "widgetshell";
    vendor = "widgetshell";
    version = "1.0";
    specVersion = "1.2";
}

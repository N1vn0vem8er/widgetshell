#include "notificationpopupscontroller.h"
#include <LayerShellQt/Window>

NotificationPopupsController::NotificationPopupsController(QObject *parent)
    : QObject{parent}
{
    connect(NotificationService::instance(), &NotificationService::notificationReceived, this, &NotificationPopupsController::showNotification);
}

void NotificationPopupsController::showNotification(const Notification &notification)
{
    NotificationPopupComponent* popup = new NotificationPopupComponent(notification);

    m_popups.append(popup);
    connect(popup, &QObject::destroyed, this, [this, popup](){
        m_popups.removeOne(popup);
        repositionPopups();
    });

    popup->show();
    repositionPopups();
}

void NotificationPopupsController::repositionPopups()
{
    int offset = 100;
    for(auto* popup : std::as_const(m_popups))
    {
        if(!popup)
            continue;

        auto* window = LayerShellQt::Window::get(popup->windowHandle());
        if(!window)
            continue;

        window->setMargins(QMargins(0, offset, 100, 0 ));
        offset += popup->height() + 10;
    }
}

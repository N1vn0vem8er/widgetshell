#include "notificationhistorycomponent.h"

NotificationHistoryComponent::NotificationHistoryComponent(QWidget *parent)
    : QWidget{parent}
{
    service = NotificationService::instance();
    mainLayout = new QVBoxLayout(this);

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setClearButtonEnabled(true);
    clearButton = new QPushButton(this);
    clearButton->setIcon(QIcon::fromTheme("edit-clear-all"));
    clearButton->setToolTip("Clear notifications");

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(clearButton);
    mainLayout->addLayout(topLayout);

    listWidget = new QListWidget(this);
    mainLayout->addWidget(listWidget);

    for(const auto& notification : service->notificationHistory())
    {
        auto* item = new QListWidgetItem(notification.body, listWidget);
        if(QUrl(notification.appIcon).isValid())
            item->setIcon(QIcon(notification.appIcon));
        else
            item->setIcon(QIcon::fromTheme(notification.appIcon));
        item->setToolTip(notification.appName);
        listWidget->addItem(item);
    }
    connect(service, &NotificationService::notificationReceived, this, &NotificationHistoryComponent::addNotification);
    connect(clearButton, &QPushButton::clicked, this, &NotificationHistoryComponent::clearNotifications);
}

void NotificationHistoryComponent::addNotification(const Notification &notification)
{
    auto* item = new QListWidgetItem(notification.body, listWidget);
    if(QUrl(notification.appIcon).isValid())
        item->setIcon(QIcon(notification.appIcon));
    else
        item->setIcon(QIcon::fromTheme(notification.appIcon));
    item->setToolTip(notification.appName);
    listWidget->addItem(item);
}

void NotificationHistoryComponent::clearNotifications()
{
    service->clearNotificationHistory();
    listWidget->clear();
}

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
        listWidget->addItem(item);
    }
    connect(service, &NotificationService::notificationReceived, this, &NotificationHistoryComponent::addNotification);
}

void NotificationHistoryComponent::addNotification(const Notification &notification)
{
    auto* item = new QListWidgetItem(notification.body, listWidget);
    listWidget->addItem(item);
}

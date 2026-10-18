#include "notificationhistorycomponent.h"

NotificationHistoryComponent::NotificationHistoryComponent(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
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
        item->setData(Qt::UserRole, QVariant::fromValue(notification));
        listWidget->addItem(item);
    }
    connect(service, &NotificationService::notificationReceived, this, &NotificationHistoryComponent::addNotification);
    connect(clearButton, &QPushButton::clicked, this, &NotificationHistoryComponent::clearNotifications);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &NotificationHistoryComponent::searchTextChanged);
}

void NotificationHistoryComponent::addNotification(const Notification &notification)
{
    auto* item = new QListWidgetItem(notification.body, listWidget);
    if(QUrl(notification.appIcon).isValid())
        item->setIcon(QIcon(notification.appIcon));
    else
        item->setIcon(QIcon::fromTheme(notification.appIcon));
    item->setToolTip(notification.appName);
    item->setData(Qt::UserRole, QVariant::fromValue(notification));
    listWidget->addItem(item);
}

void NotificationHistoryComponent::clearNotifications()
{
    service->clearNotificationHistory();
    listWidget->clear();
}

void NotificationHistoryComponent::searchTextChanged(const QString &text)
{
    for(int i = 0; i < listWidget->count(); i++)
    {
        auto* item = listWidget->item(i);
        auto notification = item->data(Qt::UserRole).value<Notification>();

        if(text.isEmpty())
        {
            item->setHidden(false);
            continue;
        }
        bool matches = notification.appName.contains(text, Qt::CaseInsensitive) || notification.summary.contains(text, Qt::CaseInsensitive) || notification.body.contains(text, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

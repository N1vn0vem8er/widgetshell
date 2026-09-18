#include "notificationhistorycomponent.h"

NotificationHistoryComponent::NotificationHistoryComponent(QWidget *parent)
    : QWidget{parent}
{
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
}

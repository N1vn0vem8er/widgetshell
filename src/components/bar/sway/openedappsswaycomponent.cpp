#include "openedappsswaycomponent.h"

OpenedAppsSwayComponent::OpenedAppsSwayComponent(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    swayService = SwayService::instance();
    desktopAppService = DesktopAppsService::instance();

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
    setLayout(mainLayout);
    updateClients();

    connect(swayService, &SwayService::clientsChanged, this, &OpenedAppsSwayComponent::updateClients);
}

void OpenedAppsSwayComponent::updateClients()
{
    if(!desktopAppService || !swayService)
        return;
    QList<DesktopApp> desktopApps = desktopAppService->desktopApps();
    const auto clients =  swayService->clients();

    const int neededButtons = clients.size();

    while(buttons.size() < neededButtons)
    {
        QPushButton* button = new QPushButton(this);
        button->setFlat(true);
        button->setCheckable(true);
        mainLayout->addWidget(button);
        buttons.append(button);
    }

    for(int i = 0; i < buttons.size(); i++)
    {
        if(i < neededButtons)
        {
            const auto& client = clients.at(i);
            QPushButton* button = buttons.at(i);
            const auto& apps = std::find_if(desktopApps.cbegin(), desktopApps.cend(), [&](const auto& app){return app.startupWMClass == client.appId || app.fileName == client.appId;});
            if(apps != desktopApps.cend())
            {
                button->setIcon(QIcon::fromTheme(apps->icon));
            }
            button->setFlat(true);
            button->setToolTip(client.title);
            button->setProperty("clientId", client.id);
            connect(button, &QPushButton::clicked, this, [this, button]{
                swayService->focusClient(button->property("clientId").toInt());
            });
            button->show();
        }
        else
        {
            QPushButton* button = buttons.takeAt(i);
            mainLayout->removeWidget(button);
            button->deleteLater();
        }
    }
}

void OpenedAppsSwayComponent::activateClient(quint64 address)
{

}

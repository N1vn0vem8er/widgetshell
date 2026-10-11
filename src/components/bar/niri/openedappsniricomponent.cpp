#include "openedappsniricomponent.h"

OpenedAppsNiriComponent::OpenedAppsNiriComponent(QWidget *parent) : OpenedAppsComponent{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    niriService = NiriService::instance();
    desktopAppService = DesktopAppsService::instance();

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
    setLayout(mainLayout);

    niriService->refreshClients();

    connect(niriService, &NiriService::clientsRefreshed, this, &OpenedAppsNiriComponent::updateClients);
    connect(niriService, &NiriService::clientFocused, this, &OpenedAppsNiriComponent::activateClient);
}

void OpenedAppsNiriComponent::updateClients()
{
    if(!desktopAppService || !niriService)
        return;
    QList<DesktopApp> desktopApps = desktopAppService->desktopApps();
    const auto clients =  niriService->clients();

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
            button->setChecked(client.isFocused);
            button->setFlat(true);
            button->setToolTip(client.title);
            button->setProperty("id", client.id);
            connect(button, &QPushButton::clicked, this, [this, button]{
                auto id = button->property("id").toULongLong();
                niriService->focusClient(id);
                activateClient(id);
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

void OpenedAppsNiriComponent::activateClient(quint64 id)
{
    for(const auto& button : std::as_const(buttons))
    {
        if(button->property("id") == id)
            button->setChecked(true);
        else
            button->setChecked(false);
    }
}

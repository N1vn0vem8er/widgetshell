#include "openedappscomponent.h"

OpenedAppsComponent::OpenedAppsComponent(QWidget *parent)
    : QWidget{parent}
{
    hyprlandService = HyprlandService::instance();
    desktopAppService = DesktopAppsService::instance();

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
    setLayout(mainLayout);
    updateClients();

    connect(hyprlandService, &HyprlandService::clientsChanged, this, &OpenedAppsComponent::updateClients);
}

OpenedAppsComponent::~OpenedAppsComponent()
{

}

void OpenedAppsComponent::updateClients()
{
    if(!desktopAppService || !hyprlandService)
        return;
    QList<DesktopApp> desktopApps = desktopAppService->desktopApps();
    QList<HyprlandClient> clients =  hyprlandService->getClients();

    const int neededButtons = clients.size();

    while(buttons.size() < neededButtons)
    {
        QPushButton* button = new QPushButton(this);
        button->setFlat(true);
        mainLayout->addWidget(button);
        buttons.append(button);
    }

    for(int i = 0; i < buttons.size(); i++)
    {
        if(i < neededButtons)
        {
            const auto& client = clients.at(i);
            QPushButton* button = buttons.at(i);
            const auto& apps = std::find_if(desktopApps.cbegin(), desktopApps.cend(), [&](const auto& app){return app.startupWMClass == client.initialClass || app.fileName == client.initialClass;});
            if(apps != desktopApps.cend())
            {
                button->setIcon(QIcon::fromTheme(apps->icon));
            }
            button->setFlat(true);
            button->setProperty("clientAddress", client.address);
            connect(button, &QPushButton::clicked, this, [this, button]{hyprlandService->focusWindow(button->property("clientAddress").toString());});
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

#include "openedappshyprlandcomponent.h"

OpenedAppsHyprlandComponent::OpenedAppsHyprlandComponent(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    hyprlandService = HyprlandService::instance();
    desktopAppService = DesktopAppsService::instance();

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
    setLayout(mainLayout);
    updateClients();
    activateClient(hyprlandService->getActiveClient().address);

    connect(hyprlandService, &HyprlandService::clientsChanged, this, &OpenedAppsHyprlandComponent::updateClients);
    connect(hyprlandService, &HyprlandService::activedWindowV2, this, &OpenedAppsHyprlandComponent::activateClient);
    connect(hyprlandService, &HyprlandService::activeWindowEmpty, this, &OpenedAppsHyprlandComponent::activeEmpty);
}

OpenedAppsHyprlandComponent::~OpenedAppsHyprlandComponent()
{

}

void OpenedAppsHyprlandComponent::updateClients()
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
            const auto& apps = std::find_if(desktopApps.cbegin(), desktopApps.cend(), [&](const auto& app){return app.startupWMClass == client.initialClass || app.fileName == client.initialClass;});
            if(apps != desktopApps.cend())
            {
                button->setIcon(QIcon::fromTheme(apps->icon));
            }
            button->setFlat(true);
            button->setToolTip(client.title);
            button->setProperty("clientAddress", client.address);
            connect(button, &QPushButton::clicked, this, [this, button]{hyprlandService->focusWindow(button->property("clientAddress").toULongLong());});
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

void OpenedAppsHyprlandComponent::activateClient(quint64 address)
{
    for(const auto& button : std::as_const(buttons))
    {
        if(button->property("clientAddress") == address)
            button->setChecked(true);
        else
            button->setChecked(false);
    }
}

void OpenedAppsHyprlandComponent::activeEmpty()
{
    for(const auto& button : std::as_const(buttons))
    {
        button->setChecked(false);
    }
}

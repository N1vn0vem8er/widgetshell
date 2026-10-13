#include "activeappnameniricomponent.h"
#include <qboxlayout.h>

ActiveAppNameNiriComponent::ActiveAppNameNiriComponent(uint maxLength, QWidget *parent) : ActiveAppNameComponent{parent}, maxLength(maxLength)
{
    service = NiriService::instance();
    label = new QLabel(this);
    auto layout = new QHBoxLayout(this);
    layout->addWidget(label);
    connect(service, &NiriService::clientOpenedOrChanged, this, &ActiveAppNameNiriComponent::activeChanged);
}

void ActiveAppNameNiriComponent::activeChanged(const NiriClient &client)
{
    label->setText(client.title.length() > maxLength ? client.title.left(maxLength) + "..." : client.title);
}

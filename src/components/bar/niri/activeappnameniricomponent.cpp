#include "activeappnameniricomponent.h"
#include <qboxlayout.h>

ActiveAppNameNiriComponent::ActiveAppNameNiriComponent(QWidget *parent) : ActiveAppNameComponent{parent}
{
    service = NiriService::instance();
    label = new QLabel(this);
    auto layout = new QHBoxLayout(this);
    layout->addWidget(label);
    connect(service, &NiriService::clientFocused, this, &ActiveAppNameNiriComponent::activeChanged);
}

void ActiveAppNameNiriComponent::activeChanged(const NiriClient &client)
{
    label->setText(client.title.length() > maxLength ? client.title.left(maxLength) + "..." : client.title);
}

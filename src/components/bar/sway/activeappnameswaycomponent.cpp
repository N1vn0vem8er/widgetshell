#include "activeappnameswaycomponent.h"
#include <qboxlayout.h>

ActiveAppNameSwayComponent::ActiveAppNameSwayComponent(uint maxLength, QWidget* parent) : ActiveAppNameComponent{parent}, maxLength(maxLength)
{
    service = SwayService::instance();
    label = new QLabel(this);
    auto layout = new QHBoxLayout(this);
    layout->addWidget(label);
    connect(service, &SwayService::clientFocused, this, &ActiveAppNameSwayComponent::activeChanged);
}

void ActiveAppNameSwayComponent::activeChanged(const SwayClient &client)
{
    label->setText(client.title.length() > maxLength ? client.title.left(maxLength) + "..." : client.title);
}

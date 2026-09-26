#include "activeappnamehyprlandcomponent.h"
#include <qboxlayout.h>

ActiveAppNameHyprlandComponent::ActiveAppNameHyprlandComponent(uint maxLength, QWidget *parent) : ActiveAppNameComponent{parent}, maxLength(maxLength)
{
    service = HyprlandService::instance();
    label = new QLabel(this);
    auto layout = new QHBoxLayout(this);
    layout->addWidget(label);
    connect(service, &HyprlandService::activedWindow, this, &ActiveAppNameHyprlandComponent::activeChanged);
}

void ActiveAppNameHyprlandComponent::activeChanged(const QString &, const QString &windowTitle)
{
    label->setText(windowTitle.length() > maxLength ? windowTitle.left(maxLength) + "..." : windowTitle);
}

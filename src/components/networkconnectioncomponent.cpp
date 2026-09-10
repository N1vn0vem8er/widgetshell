#include "networkconnectioncomponent.h"
#include <qboxlayout.h>

NetworkConnectionComponent::NetworkConnectionComponent(QWidget *parent)
    : QWidget{parent}
{
    service = NetworkService::instance();
    connectionTypeLabel = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(connectionTypeLabel);
}

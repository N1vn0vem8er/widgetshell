#include "networkmenu.h"
#include <LayerShellQt/window.h>
#include <QVBoxLayout>

NetworkMenu::NetworkMenu(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 600);
    winId();
    layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft));

    service = NetworkService::instance();

    ipAddressLabel = new QLabel(this);
    gatewayAddressLabel = new QLabel(this);

    ipList = new QListWidget(this);
    for(const auto& i : service->getIpAddresses().asKeyValueRange())
    {
        ipList->addItem(new QListWidgetItem(QString("%1: %2").arg(i.first, i.second), ipList));
    }
    ipList->addItem(new QListWidgetItem(QString("Default Gateway: %1").arg(service->getGatewayAddress()), ipList));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(ipList);
}

void NetworkMenu::showForWidget(QWidget* widget)
{
    QPoint position = widget->mapToGlobal(QPoint(0, widget->height()));
    layerWindow->setMargins(QMargins(position.x(), 0, 0, position.y()));
    show();
}

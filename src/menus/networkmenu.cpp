#include "networkmenu.h"
#include <LayerShellQt/window.h>
#include <QFormLayout>
#include <QPushButton>
#include <QScrollArea>
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

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(4);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* interfacesWidget = new QWidget(this);
    QVBoxLayout* interfacesLayout = new QVBoxLayout(interfacesWidget);
    interfacesLayout->setContentsMargins(0, 0, 0, 0);
    interfacesLayout->setSpacing(4);

    const auto interfaces = service->networkInterfaces();

    for(auto it = interfaces.cbegin(); it != interfaces.cend(); it++)
    {
        const NetworkInterface& iface = it.value();
        QWidget* interfaceContainer = new QWidget(this);
        QVBoxLayout* interfaceLayout = new QVBoxLayout(interfaceContainer);
        interfaceLayout->setContentsMargins(0, 0, 0, 0);
        interfaceLayout->setSpacing(0);
        QPushButton* button = new QPushButton(interfaceContainer);
        QString status;

        if(!iface.up)
            status = "down";
        else if(!iface.carrier)
            status = "no carrier";
        else
            status = "up";

        QString title = iface.name;

        if(!iface.ssid.isEmpty())
            title += QString("  •  %1").arg(iface.ssid);

        button->setText(QString("%1  [%2]").arg(title, status));
        button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        interfaceLayout->addWidget(button);

        QFrame* details = new QFrame(interfaceContainer);
        details->setFrameShape(QFrame::StyledPanel);
        details->setVisible(false);

        QFormLayout* detailsLayout = new QFormLayout(details);
        detailsLayout->setContentsMargins(12, 8, 12, 8);
        detailsLayout->addRow("Name:", valueLabel(iface.name));
        detailsLayout->addRow("Status:",valueLabel(iface.up ? (iface.carrier ? "up / carrier" : "up / no carrier") : "down"));
        detailsLayout->addRow("MAC:", valueLabel(iface.macAddress));
        detailsLayout->addRow("IPv4:", valueLabel(iface.ipv4Addresses.join(", ")));
        detailsLayout->addRow("IPv6:", valueLabel(iface.ipv6Addresses.join(", ")));
        detailsLayout->addRow("Gateway:", valueLabel(iface.gateway));
        detailsLayout->addRow("MTU:", valueLabel(QString::number(iface.mtu)));
        detailsLayout->addRow("Speed:", valueLabel(QString::number(iface.speed) + " Mbps"));
        detailsLayout->addRow("Duplex:", valueLabel(iface.duplex));

        if(!iface.ssid.isEmpty())
        {
            detailsLayout->addRow("SSID:", valueLabel(iface.ssid));
            detailsLayout->addRow("Signal:", valueLabel(QString::number(iface.signalDbm) + " dBm"));
            detailsLayout->addRow("Frequency:", valueLabel(QString::number(iface.frequency) + " MHz"));
            detailsLayout->addRow("RX bitrate:", valueLabel(QString::number(iface.rxBitrate) + " Mbps"));
            detailsLayout->addRow("TX bitrate:", valueLabel(QString::number(iface.txBitrate) + " Mbps"));
        }

        detailsLayout->addRow("RX:", valueLabel(formatBytes(iface.rxBytes)));
        detailsLayout->addRow("TX:", valueLabel(formatBytes(iface.txBytes)));
        detailsLayout->addRow("RX packets:", valueLabel(QString::number(iface.rxPackets)));
        detailsLayout->addRow("TX packets:", valueLabel(QString::number(iface.txPackets)));
        detailsLayout->addRow("RX errors:", valueLabel(QString::number(iface.rxErrors)));
        detailsLayout->addRow("TX errors:", valueLabel(QString::number(iface.txErrors)));
        detailsLayout->addRow("RX dropped:", valueLabel(QString::number(iface.rxDropped)));
        detailsLayout->addRow("TX dropped:", valueLabel(QString::number(iface.txDropped)));
        interfaceLayout->addWidget(details);

        connect(button, &QPushButton::clicked, this,[details, button](){
            const bool visible = !details->isVisible();
            details->setVisible(visible);
        });

        interfacesLayout->addWidget(interfaceContainer);
    }

    interfacesLayout->addStretch();

    scrollArea->setWidget(interfacesWidget);
    mainLayout->addWidget(scrollArea);
}

void NetworkMenu::showForWidget(QWidget* widget)
{
    QPoint position = widget->mapToGlobal(QPoint(0, widget->height()));
    layerWindow->setMargins(QMargins(position.x(), 0, 0, position.y()));
    show();
}

QString NetworkMenu::formatBytes(quint64 bytes)
{
    const double b = static_cast<double>(bytes);

    if(b >= 1024.0 * 1024.0 * 1024.0)
        return QString::number(b / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
    if(b >= 1024.0 * 1024.0)
        return QString::number(b / (1024.0 * 1024.0), 'f', 2) + " MB";
    if(b >= 1024.0)
        return QString::number(b / 1024.0, 'f', 2) + " KB";

    return QString::number(bytes) + " B";
}

QLabel *NetworkMenu::valueLabel(const QString &value)
{
    QLabel* label = new QLabel(value);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    return label;
}

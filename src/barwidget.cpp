#include "barwidget.h"
#include <LayerShellQt/Window>
#include <QDateTime>

BarWidget::BarWidget(QWidget *parent)
    : QWidget{parent}
{
    resize(100, 30);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerTop);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft | LayerShellQt::Window::AnchorRight));
    layerWindow->setExclusiveZone(height());

    mainLayout = new QHBoxLayout(this);

    spacerItem = new QSpacerItem(1046, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    mainLayout->addItem(spacerItem);
    dateTimeLabel = new QLabel(this);
    mainLayout->addWidget(dateTimeLabel);
    dateTimeTimer = new QTimer(this);
    connect(dateTimeTimer, &QTimer::timeout, this, [this]{dateTimeLabel->setText(QDateTime::currentDateTime().toString("hh::mm:ss | dd:MM:yyyy"));});
    dateTimeTimer->setInterval(1000);
    dateTimeTimer->start();
}

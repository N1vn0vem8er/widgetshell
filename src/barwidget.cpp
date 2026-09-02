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

    mainLayout = new QGridLayout(this);

    mainLayout->addWidget(new QWidget(this), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    workspaceSwitcher = new WorkspaceSwitcherComponent(this);
    mainLayout->addWidget(workspaceSwitcher, 0, 1, Qt::AlignCenter);

    controlMenuButton = new QPushButton(QIcon::fromTheme("application-menu"), "", this);
    dateTimeLabel = new QLabel(this);
    QWidget* rightWidget = new QWidget(this);
    QHBoxLayout* rightLayout = new QHBoxLayout(rightWidget);
    rightLayout->addWidget(dateTimeLabel);
    rightLayout->addWidget(controlMenuButton);

    mainLayout->addWidget(rightWidget, 0, 2, Qt::AlignRight | Qt::AlignVCenter);
    dateTimeTimer = new QTimer(this);
    connect(dateTimeTimer, &QTimer::timeout, this, [this]{dateTimeLabel->setText(QDateTime::currentDateTime().toString("hh::mm:ss | dd:MM:yyyy"));});
    dateTimeTimer->setInterval(1000);
    dateTimeTimer->start();
}

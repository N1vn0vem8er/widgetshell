#include "diskusagebarcomponent.h"
#include <LayerShellQt/window.h>
#include <qboxlayout.h>
#include <qlistwidget.h>

DiskUsageBarComponent::DiskUsageBarComponent(const QString &diskName, const QString &labelText, DiskUsageBarComponent::Unit unit, QWidget *parent)
    : QPushButton{parent}, diskName(diskName), labelText(labelText), unit(unit)
{
    setAttribute(Qt::WA_StyledBackground, true);
    service = SystemMonitorService::instance();
    setFlat(true);

    connect(service, &SystemMonitorService::diskStatsChanged, this, &DiskUsageBarComponent::diskStatsChanged);
    connect(this, &QPushButton::clicked, this, &DiskUsageBarComponent::onSelectDisk);
}

DiskUsageBarComponent::~DiskUsageBarComponent()
{
    if(selectDiskWidget)
        selectDiskWidget->deleteLater();
}

void DiskUsageBarComponent::onSelectDisk()
{
    if(!selectDiskWidget)
    {
        selectDiskWidget = new QWidget();

        auto* layout = new QVBoxLayout(selectDiskWidget);
        listWidget = new QListWidget(selectDiskWidget);
        listWidget->setFrameShape(QFrame::NoFrame);
        listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        layout->addWidget(listWidget);

        connect(listWidget, &QListWidget::itemClicked, this, [this](auto* item){
            diskName = item->data(Qt::UserRole).toString();
        });

        selectDiskWidget->setFixedSize(200, 300);
        selectDiskWidget->winId();
        LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(selectDiskWidget->windowHandle());
        layerWindow->setLayer(LayerShellQt::Window::Layer::LayerTop);
        layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft));
        QPoint position = mapToGlobal(QPoint(0, height()));
        layerWindow->setMargins(QMargins(position.x(), 0, 0, position.y()));
    }
    if(!selectDiskWidget->isVisible())
    {
        listWidget->clear();

        for(const auto& disk : std::as_const(disks))
        {
            auto* item = new QListWidgetItem(disk);
            item->setData(Qt::UserRole, disk);
            listWidget->addItem(item);
        }

        if(listWidget->count() == 0)
        {
            auto* item = new QListWidgetItem("No disks");
            item->setFlags(Qt::NoItemFlags);
            listWidget->addItem(item);
        }
        selectDiskWidget->show();
    }
    else
    {
        selectDiskWidget->hide();
    }
}

void DiskUsageBarComponent::diskStatsChanged(const QMap<QString, DiskStats> &stats)
{
    disks = stats.keys();
    if(stats.contains(diskName))
    {
        auto disk = stats.value(diskName);
        auto convert = [this](quint64 value) -> double{
            switch(unit)
            {
            case Unit::KB:
                return value;
            case Unit::MB:
                return value / 1024.0;
            case Unit::GB:
                return value / (1024.0 * 1024.0);
            case Unit::TB:
                return value / (1024.0 * 1024.0 * 1024.0);
            }
            return value;
        };
        const QString unitText = [this](){
            switch(unit)
            {
            case Unit::KB:
                return "KB";
            case Unit::MB:
                return "MB";
            case Unit::GB:
                return "GB";
            case Unit::TB:
                return "TB";
            }
            return "KB";
        }();

        setText(labelText.arg(disk.name).arg(convert(disk.read), 0, 'f', 2).arg(convert(disk.write), 0, 'f', 2).arg(unitText));
    }
}

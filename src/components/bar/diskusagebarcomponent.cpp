#include "diskusagebarcomponent.h"
#include <qboxlayout.h>

DiskUsageBarComponent::DiskUsageBarComponent(QWidget *parent)
    : QWidget{parent}
{
    service = SystemMonitorService::instance();

    label = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);

    connect(service, &SystemMonitorService::diskStatsChanged, this, &DiskUsageBarComponent::diskStatsChanged);
}

void DiskUsageBarComponent::diskStatsChanged(const QList<DiskStats> &stats)
{
    if(!stats.isEmpty())
    {
        auto disk = stats.first();
        label->setText(QString("%1 %2 %3").arg(disk.name).arg(disk.read).arg(disk.write));
    }
}

#include "diskusagebarcomponent.h"
#include <qboxlayout.h>

DiskUsageBarComponent::DiskUsageBarComponent(const QString &diskName, const QString &labelText, DiskUsageBarComponent::Unit unit, QWidget *parent)
    : QPushButton{parent}, diskName(diskName), labelText(labelText), unit(unit)
{
    service = SystemMonitorService::instance();
    setFlat(true);

    connect(service, &SystemMonitorService::diskStatsChanged, this, &DiskUsageBarComponent::diskStatsChanged);
}

void DiskUsageBarComponent::diskStatsChanged(const QMap<QString, DiskStats> &stats)
{
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

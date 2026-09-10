#include "memoryusagecomponent.h"
#include <qboxlayout.h>

MemoryUsageComponent::MemoryUsageComponent(Unit unit, QWidget *parent)
    : QWidget{parent}
{
    this->unit = unit;
    service = SystemMonitorService::instance();
    label = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);
    connect(service, &SystemMonitorService::memoryUsage, this, &MemoryUsageComponent::update);
}

void MemoryUsageComponent::update(quint64 used, quint64 free)
{
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

    label->setText(QString("%1 %3 / %2 %3").arg(convert(used), 0, 'f', 2).arg(convert(used + free), 0, 'f', 2).arg(unitText));
}

#include "storageusagebarcomponent.h"
#include <qboxlayout.h>

StorageUsageBarComponent::StorageUsageBarComponent(const QString &labelText, StorageUsageBarComponent::Unit unit, QWidget *parent)
    : unit(unit), labelText(labelText), QWidget{parent}
{
    service = SystemMonitorService::instance();
    label = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);
    updateLabel();
}

void StorageUsageBarComponent::updateLabel()
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

    auto disk = service->checkStorage(path);
    label->setText(labelText.arg(convert(disk.total), 0, 'f', 2).arg(convert(disk.available), 0, 'f', 2).arg(unitText));
}

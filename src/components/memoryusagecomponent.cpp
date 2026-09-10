#include "memoryusagecomponent.h"
#include <qboxlayout.h>

MemoryUsageComponent::MemoryUsageComponent(QWidget *parent)
    : QWidget{parent}
{
    service = SystemMonitorService::instance();
    label = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);
    connect(service, &SystemMonitorService::memoryUsage, this, &MemoryUsageComponent::update);
}

void MemoryUsageComponent::update(quint64 used, quint64 free)
{
    label->setText(QString("%1 KB / %2 KB").arg(used).arg(used + free));
}

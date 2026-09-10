#include "cpuusagecomponent.h"
#include <qboxlayout.h>

CpuUsageComponent::CpuUsageComponent(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    service = SystemMonitorService::instance();
    label = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);
    connect(service, &SystemMonitorService::cpuUsage, this, &CpuUsageComponent::update);
}

void CpuUsageComponent::update(double val)
{
    label->setText(QString("%1%").arg(val));
}

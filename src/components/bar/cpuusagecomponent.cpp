#include "cpuusagecomponent.h"
#include <qboxlayout.h>

CpuUsageComponent::CpuUsageComponent(const QString &labelText, QWidget *parent)
    : labelText(labelText), QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    service = SystemMonitorService::instance();
    label = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);
    connect(service, &SystemMonitorService::cpuUsage, this, &CpuUsageComponent::update);
}

void CpuUsageComponent::update(const QList<double>& usage)
{
    label->setText(labelText.arg(usage.at(0), 0, 'f', 1));
}

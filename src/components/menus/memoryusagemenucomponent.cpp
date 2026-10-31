#include "memoryusagemenucomponent.h"

MemoryUsageMenuComponent::MemoryUsageMenuComponent(ChartType chartType, QWidget *parent)
    : QWidget{parent}, chartType(chartType)
{
    service = SystemMonitorService::instance();
}

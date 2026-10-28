#include "cpuusagemenucomponent.h"
#include <qboxlayout.h>

CpuUsageMenuComponent::CpuUsageMenuComponent(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumHeight(180);
    service = SystemMonitorService::instance();
    connect(service, &SystemMonitorService::cpuUsage, this, &CpuUsageMenuComponent::update);

    series = new QPieSeries(this);

    usedSlice = series->append("CPU usage", 0);
    freeSlice = series->append("Free", 100);

    usedSlice->setBrush(QColor(0x4CAF50));
    freeSlice->setBrush(QColor(0x303030));

    chart = new QChart();
    chart->addSeries(series);
    chart->setBackgroundVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setTitle(QString());

    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
}

void CpuUsageMenuComponent::update(double val)
{
    usedSlice->setValue(val);
    freeSlice->setValue(100.0 - val);

    if(val >= 90.0)
        usedSlice->setBrush(QColor(0xF44336));
    else if(val >= 70.0)
        usedSlice->setBrush(QColor(0xFF9800));
    else
        usedSlice->setBrush(QColor(0x4CAF50));
}

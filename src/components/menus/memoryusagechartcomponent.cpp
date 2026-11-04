#include "memoryusagechartcomponent.h"
#include <qboxlayout.h>

MemoryUsageChartComponent::MemoryUsageChartComponent(ChartType chartType, QWidget *parent)
    : QWidget{parent}, chartType(chartType)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumHeight(180);
    service = SystemMonitorService::instance();
    connect(service, &SystemMonitorService::memoryUsage, this, &MemoryUsageChartComponent::update);

    chart = new QChart();
    chart->setBackgroundVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));

    switch(chartType)
    {
    case ChartType::Pie:
    {
        series = new QPieSeries(this);
        usedSlice = series->append("Memory usage", 0);
        freeSlice = series->append("Free", 100);
        usedSlice->setBrush(QColor(0x4CAF50));
        freeSlice->setBrush(QColor(0x303030));
        chart->addSeries(series);
        break;
    }

    case ChartType::Line:
    {
        lineSeries = new QLineSeries(this);
        lineSeries->setName("CPU usage");
        lineSeries->setColor(QColor(0x4CAF50));
        chart->addSeries(lineSeries);
        axisX = new QValueAxis(this);
        axisY = new QValueAxis(this);
        axisX->setTitleText("Samples");
        axisY->setTitleText("Memory usage");
        axisX->setRange(0, 60);
        axisY->setRange(0, 100);
        axisX->setLabelFormat("%d");
        axisY->setLabelFormat("%.0f");
        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);
        lineSeries->attachAxis(axisX);
        lineSeries->attachAxis(axisY);
        break;
    }

    case ChartType::Bar:
    {
        barSeries = new QBarSeries(this);
        barSet = new QBarSet("Memory usage");
        barSet->setColor(QColor(0x4CAF50));
        barSeries->append(barSet);
        chart->addSeries(barSeries);
        axisY = new QValueAxis(this);
        axisY->setRange(0, 100);
        axisY->setLabelFormat("%.0f");
        axisY->setTitleText("Memory usage");
        chart->addAxis(axisY, Qt::AlignLeft);
        barSeries->attachAxis(axisY);
        break;
    }
    }

    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
}

void MemoryUsageChartComponent::update(quint64 used, quint64 free)
{

    double val = (static_cast<double>(used) / (static_cast<double>(used) + static_cast<double>(free))) * 100;
    QColor color;

    if(val >= 90.0)
        color = QColor(0xF44336);
    else if(val >= 70.0)
        color = QColor(0xFF9800);
    else
        color = QColor(0x4CAF50);

    switch(chartType)
    {
    case ChartType::Pie:
    {
        usedSlice->setValue(val);
        freeSlice->setValue(100.0 - val);
        usedSlice->setBrush(color);
        break;
    }

    case ChartType::Line:
    {
        lineSeries->append(sampleCount, val);
        if(lineSeries->count() > 60)
            lineSeries->remove(0);
        sampleCount++;
        if(sampleCount > 60)
            axisX->setRange(sampleCount - 60, sampleCount);
        lineSeries->setColor(color);
        break;
    }

    case ChartType::Bar:
    {
        *barSet << val;
        if(barSet->count() > 10)
            barSet->remove(0);
        barSet->setColor(color);
        break;
    }
    }
}

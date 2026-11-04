#ifndef MEMORYUSAGECHARTCOMPONENT_H
#define MEMORYUSAGECHARTCOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>
#include <qbarseries.h>
#include <qbarset.h>
#include <qchart.h>
#include <qchartview.h>
#include <qlineseries.h>
#include <qpieseries.h>
#include <qvalueaxis.h>

class MemoryUsageChartComponent : public QWidget
{
    Q_OBJECT
public:
    enum class ChartType {
        Pie,
        Line,
        Bar
    };
    Q_ENUM(ChartType)
    explicit MemoryUsageChartComponent(MemoryUsageChartComponent::ChartType chartType, QWidget *parent = nullptr);

private:
    SystemMonitorService* service {nullptr};
    MemoryUsageChartComponent::ChartType chartType {ChartType::Pie};
    QChart* chart {nullptr};
    QChartView* chartView {nullptr};
    QPieSeries* series {nullptr};
    QPieSlice* usedSlice {nullptr};
    QPieSlice* freeSlice {nullptr};
    QLineSeries* lineSeries {nullptr};
    QBarSeries* barSeries {nullptr};
    QBarSet* barSet {nullptr};
    QValueAxis* axisX {nullptr};
    QValueAxis* axisY {nullptr};
    int sampleCount {0};

private slots:
    void update(quint64 used, quint64 free);

signals:
};

#endif // MEMORYUSAGECHARTCOMPONENT_H

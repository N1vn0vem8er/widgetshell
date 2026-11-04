#ifndef CPUUSAGECHARTOMPONENT_H
#define CPUUSAGECHARTOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QBarSeries>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QPieSeries>
#include <QValueAxis>
#include <QWidget>

class CpuUsageChartComponent : public QWidget
{
    Q_OBJECT
public:
    enum class ChartType {
        Pie,
        Line,
        Bar
    };
    Q_ENUM(ChartType)
    explicit CpuUsageChartComponent(CpuUsageChartComponent::ChartType chartType, QWidget *parent = nullptr);

private:
    SystemMonitorService* service {nullptr};
    QChart* chart {nullptr};
    ChartType chartType = ChartType::Pie;
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
    void update(const QList<double> &usage);

signals:
};

#endif // CPUUSAGECHARTOMPONENT_H

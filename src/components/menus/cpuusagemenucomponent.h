#ifndef CPUUSAGEMENUCOMPONENT_H
#define CPUUSAGEMENUCOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QWidget>

class CpuUsageMenuComponent : public QWidget
{
    Q_OBJECT
public:
    explicit CpuUsageMenuComponent(QWidget *parent = nullptr);

private:
    SystemMonitorService* service;
    QChart* chart;
    QChartView* chartView;
    QPieSeries* series;
    QPieSlice* usedSlice;
    QPieSlice* freeSlice;

private slots:
    void update(const QList<double> &usage);

signals:
};

#endif // CPUUSAGEMENUCOMPONENT_H

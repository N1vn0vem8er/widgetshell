#ifndef CPUUSAGECOMPONENT_H
#define CPUUSAGECOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>
#include <qlabel.h>

class CpuUsageComponent : public QWidget
{
    Q_OBJECT
public:
    explicit CpuUsageComponent(QWidget *parent = nullptr);

private:
    SystemMonitorService* service;
    QLabel* label;

private slots:
    void update(double val);

signals:
};

#endif // CPUUSAGECOMPONENT_H

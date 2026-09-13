#ifndef CPUUSAGECOMPONENT_H
#define CPUUSAGECOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>
#include <qlabel.h>

class CpuUsageComponent : public QWidget
{
    Q_OBJECT
public:
    explicit CpuUsageComponent(const QString& labelText, QWidget *parent = nullptr);

private:
    SystemMonitorService* service;
    QLabel* label;
    QString labelText;

private slots:
    void update(double val);

signals:
};

#endif // CPUUSAGECOMPONENT_H

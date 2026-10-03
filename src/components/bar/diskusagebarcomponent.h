#ifndef DISKUSAGEBARCOMPONENT_H
#define DISKUSAGEBARCOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>
#include <qlabel.h>

class DiskUsageBarComponent : public QWidget
{
    Q_OBJECT
public:
    explicit DiskUsageBarComponent(QWidget *parent = nullptr);

private:
    QLabel* label;
    SystemMonitorService* service;

private slots:
    void diskStatsChanged(const QList<DiskStats>& stats);

signals:
};

#endif // DISKUSAGEBARCOMPONENT_H

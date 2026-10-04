#ifndef DISKUSAGEBARCOMPONENT_H
#define DISKUSAGEBARCOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>
#include <qpushbutton.h>

class DiskUsageBarComponent : public QPushButton
{
    Q_OBJECT
public:
    enum class Unit {
        KB,
        MB,
        GB,
        TB
    };
    explicit DiskUsageBarComponent(const QString& diskName, const QString& labelText, DiskUsageBarComponent::Unit unit, QWidget *parent = nullptr);

private:
    SystemMonitorService* service;
    Unit unit;
    QString labelText {"%1 ↑ %2 %4 | ↓ %3 %4"};
    QString diskName;

private slots:
    void diskStatsChanged(const QMap<QString, DiskStats>& stats);

signals:
};

#endif // DISKUSAGEBARCOMPONENT_H

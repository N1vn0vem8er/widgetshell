#ifndef MEMORYUSAGECOMPONENT_H
#define MEMORYUSAGECOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>
#include <qlabel.h>

class MemoryUsageComponent : public QWidget
{
    Q_OBJECT
public:
    enum class Unit {
        KB,
        MB,
        GB,
        TB
    };
    explicit MemoryUsageComponent(const QString& labelText, MemoryUsageComponent::Unit unit, QWidget *parent = nullptr);

private:
    SystemMonitorService* service;
    QLabel* label;
    QString labelText;
    Unit unit {Unit::KB};

private slots:
    void update(quint64 used, quint64 free);

signals:
};

#endif // MEMORYUSAGECOMPONENT_H

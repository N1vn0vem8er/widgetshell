#ifndef MEMORYUSAGEMENUCOMPONENT_H
#define MEMORYUSAGEMENUCOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>

class MemoryUsageMenuComponent : public QWidget
{
    Q_OBJECT
public:
    enum class ChartType {
        Pie,
        Line,
        Bar
    };
    Q_ENUM(ChartType)
    explicit MemoryUsageMenuComponent(MemoryUsageMenuComponent::ChartType chartType, QWidget *parent = nullptr);

private:
    SystemMonitorService* service {nullptr};
    MemoryUsageMenuComponent::ChartType chartType;

signals:
};

#endif // MEMORYUSAGEMENUCOMPONENT_H

#ifndef MEMORYUSAGECOMPONENT_H
#define MEMORYUSAGECOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>
#include <qlabel.h>

class MemoryUsageComponent : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryUsageComponent(QWidget *parent = nullptr);

private:
    SystemMonitorService* service;
    QLabel* label;

private slots:
    void update(quint64 used, quint64 free);

signals:
};

#endif // MEMORYUSAGECOMPONENT_H

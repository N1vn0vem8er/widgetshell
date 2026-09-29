#ifndef STORAGEUSAGEBARCOMPONENT_H
#define STORAGEUSAGEBARCOMPONENT_H

#include "services/systemmonitorservice.h"
#include <QWidget>
#include <qlabel.h>

class StorageUsageBarComponent : public QWidget
{
    Q_OBJECT
public:
    enum class Unit {
        KB,
        MB,
        GB,
        TB
    };
    explicit StorageUsageBarComponent(const QString& labelText, StorageUsageBarComponent::Unit unit, QWidget *parent = nullptr);

private:
    SystemMonitorService* service;
    QLabel* label;
    Unit unit;
    QString labelText {"%1%6 / %2%6"};
    QString path {"/"};

private slots:
    void updateLabel();

signals:
};

#endif // STORAGEUSAGEBARCOMPONENT_H

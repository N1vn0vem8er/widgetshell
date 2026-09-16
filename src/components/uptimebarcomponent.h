#ifndef UPTIMEBARCOMPONENT_H
#define UPTIMEBARCOMPONENT_H

#include <QTimer>
#include <QWidget>
#include <qlabel.h>

class UptimeBarComponent : public QWidget
{
    Q_OBJECT
public:
    explicit UptimeBarComponent(QWidget *parent = nullptr);

private:
    QLabel* label;
    QTimer* timer;
    long long uptime;

signals:
};

#endif // UPTIMEBARCOMPONENT_H

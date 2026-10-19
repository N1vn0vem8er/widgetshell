#ifndef UPTIMEBARCOMPONENT_H
#define UPTIMEBARCOMPONENT_H

#include <QTimer>
#include <QWidget>
#include <qlabel.h>

class UptimeBarComponent : public QWidget
{
    Q_OBJECT
public:
    explicit UptimeBarComponent(const QString& format, QWidget *parent = nullptr);

private:
    QLabel* label;
    QTimer* timer;
    QString format{"%1d %2h %3m %4s"};
    long long uptime;

signals:
};

#endif // UPTIMEBARCOMPONENT_H

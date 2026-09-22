#ifndef DATETIMECOMPONENT_H
#define DATETIMECOMPONENT_H

#include <QWidget>
#include <qboxlayout.h>
#include <qlabel.h>

class DateTimeComponent : public QWidget
{
    Q_OBJECT
public:
    explicit DateTimeComponent(const QString& format, QWidget *parent = nullptr);

private:
    QHBoxLayout* mainLayout;
    QLabel* label;
    QTimer* dateTimeTimer;
    QString format{"hh::mm:ss | dd:MM:yyyy"};

signals:
};

#endif // DATETIMECOMPONENT_H

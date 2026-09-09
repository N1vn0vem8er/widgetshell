#include "datetimecomponent.h"
#include <QDateTime>
#include <qtimer.h>

DateTimeComponent::DateTimeComponent(const QString& format, QWidget *parent)
    : QWidget{parent}
{
    this->format = format;
    mainLayout = new QHBoxLayout(this);
    label = new QLabel(this);
    mainLayout->addWidget(label);
    dateTimeTimer = new QTimer(this);
    connect(dateTimeTimer, &QTimer::timeout, this, [this]{label->setText(QDateTime::currentDateTime().toString(this->format));});
    dateTimeTimer->setInterval(1000);
    dateTimeTimer->start();
}

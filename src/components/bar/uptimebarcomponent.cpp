#include "uptimebarcomponent.h"
#include <qboxlayout.h>
#include <sys/sysinfo.h>

UptimeBarComponent::UptimeBarComponent(const QString &format, QWidget *parent)
    : QWidget{parent}, format(format)
{
    setAttribute(Qt::WA_StyledBackground, true);
    label = new QLabel(this);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);
    struct sysinfo info;

    if(sysinfo(&info) == 0)
        uptime = info.uptime;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]{
        uptime++;
        long long seconds = uptime;
        long long days = seconds / 86400;
        seconds %= 86400;
        long long hours = seconds / 3600;
        seconds %= 3600;
        long long minutes = seconds / 60;
        seconds %= 60;
        label->setText(this->format.arg(days).arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
    });
    timer->setInterval(1000);
    timer->start();
}

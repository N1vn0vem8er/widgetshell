#include "uptimebarcomponent.h"
#include <qboxlayout.h>
#include <sys/sysinfo.h>

UptimeBarComponent::UptimeBarComponent(QWidget *parent)
    : QWidget{parent}
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
        label->setText(QString("%1d %2h %3m %4s").arg(days).arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
    });
    timer->setInterval(1000);
    timer->start();
}

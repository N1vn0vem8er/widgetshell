#include "systemmonitorservice.h"

#include <QFile>
#include <QTextStream>

SystemMonitorService *SystemMonitorService::instance()
{
    return m_instance;
}

SystemMonitorService *SystemMonitorService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new SystemMonitorService(parent);
    return m_instance;
}

void SystemMonitorService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

SystemMonitorService::SystemMonitorService(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SystemMonitorService::timeout);
    timer->setInterval(1000);
    timer->start();
}

CpuTimes SystemMonitorService::readCpuTimes()
{
    QFile file("/proc/stat");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    const QString line = QString::fromUtf8(file.readLine());
    QTextStream stream(const_cast<QString*>(&line), QIODevice::ReadOnly);
    QString cpu;
    CpuTimes times;

    stream >> cpu >> times.user >> times.nice >> times.system >> times.idle >> times.iowait >> times.irq >> times.softirq >> times.steal >> times.guest >> times.guestNice;
    return times;
}

double SystemMonitorService::countCpuUsage(const CpuTimes &old, const CpuTimes &current)
{
    const quint64 oldIdle = old.idle + old.iowait;
    const quint64 currentIdle = current.idle + current.iowait;
    const quint64 oldTotal = old.user + old.nice + old.system + old.idle + old.iowait + old.irq + old.softirq + old.steal + old.guest + old.guestNice;
    const quint64 currentTotal = current.user + current.nice + current.system + current.idle + current.iowait + current.irq + current.softirq + current.steal + current.guest + current.guestNice;
    const quint64 deltaTotal = currentTotal - oldTotal;
    const quint64 deltaIdle = currentIdle - oldIdle;
    if(deltaTotal == 0)
        return 0.0;

    return 100.0 * static_cast<double>(deltaTotal - deltaIdle) / static_cast<double>(deltaTotal);
}

void SystemMonitorService::checkCpu()
{
    const CpuTimes current = readCpuTimes();
    emit cpuUsage(countCpuUsage(previousCpuTimes, current));
    previousCpuTimes = current;
}

void SystemMonitorService::checkMemory()
{
    QFile file("/proc/meminfo");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    const QByteArray data = file.readAll();
    file.close();

    quint64 total = 0;
    quint64 available = 0;
    const QList<QByteArray> lines = data.split('\n');

    for(const QByteArray &line : lines)
    {
        const QList<QByteArray> parts = line.simplified().split(' ');
        if(parts.size() < 2)
            continue;
        if(parts[0] == "MemTotal:")
            total = parts[1].toULongLong();
        else if(parts[0] == "MemAvailable:")
            available = parts[1].toULongLong();
    }

    if(total == 0)
        return;
    emit memoryUsage(total - available, available);
}

void SystemMonitorService::timeout()
{
    checkCpu();
    checkMemory();
}

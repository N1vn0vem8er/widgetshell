#include "systemmonitorservice.h"

#include <QFile>
#include <QRegularExpression>
#include <QTextStream>
#include <qdatetime.h>
#include <sys/statvfs.h>

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
    m_previousTime = QDateTime::currentMSecsSinceEpoch();
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

    for(const QByteArray&  line : lines)
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

void SystemMonitorService::updateDiskStats()
{
    QFile file("/proc/diskstats");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    const qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    const double elapsedSeconds = (currentTime - m_previousTime) / 1000.0;

    if(elapsedSeconds <= 0.0)
        return;

    m_previousTime = currentTime;

    QList<DiskStats> result;
    const QByteArray data = file.readAll();
    file.close();
    const QList<QByteArray> lines = data.split('\n');

    for(const auto& line : lines)
    {
        if(line.isEmpty())
            continue;

        static QRegularExpression regex("\\s+");
        const QStringList fields = QString::fromUtf8(line).split(regex);

        if(fields.size() < 10)
            continue;

        const QString deviceName = fields[3];
        const quint64 readSectors = fields[5].toULongLong();
        const quint64 writeSectors = fields[9].toULongLong();

        DiskStats current;
        current.read = readSectors;
        current.write = writeSectors;

        if(m_previousStats.contains(deviceName))
        {
            const DiskStats previous = m_previousStats.value(deviceName);
            const quint64 readDelta = readSectors >= previous.read ? readSectors - previous.read : 0;
            const quint64 writeDelta = writeSectors >= previous.write ? writeSectors - previous.write : 0;

            DiskStats stats;
            stats.name = deviceName;
            stats.read = (readDelta * 512.0) / elapsedSeconds;
            stats.write = (writeDelta * 512.0) / elapsedSeconds;
            result.append(stats);
        }

        m_previousStats[deviceName] = current;
    }

    emit diskStatsChanged(result);
}

Disk SystemMonitorService::checkStorage(const QString &path)
{
    struct statvfs fs;
    statvfs(path.toUtf8(), &fs);
    Disk disk;
    disk.path = path;
    disk.total = static_cast<unsigned long long>(fs.f_blocks) * fs.f_frsize;
    disk.free = static_cast<unsigned long long>(fs.f_bfree) * fs.f_frsize;
    disk.available = static_cast<unsigned long long>(fs.f_bavail) * fs.f_frsize;
    return disk;
}

void SystemMonitorService::timeout()
{
    checkCpu();
    checkMemory();
    updateDiskStats();
}

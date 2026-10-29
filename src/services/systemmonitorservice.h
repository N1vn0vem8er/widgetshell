#ifndef SYSTEMMONITORSERVICE_H
#define SYSTEMMONITORSERVICE_H

#include <QMap>
#include <QObject>
#include <qtimer.h>

struct CpuTimes
{
    quint64 user {0};
    quint64 nice {0};
    quint64 system {0};
    quint64 idle {0};
    quint64 iowait {0};
    quint64 irq {0};
    quint64 softirq {0};
    quint64 steal {0};
    quint64 guest {0};
    quint64 guestNice {0};
};

struct Disk {
    QString path;
    unsigned long long total;
    unsigned long long free;
    unsigned long long available;
};

struct DiskStats
{
    QString name;
    double read {0.0};
    double write {0.0};
};

class SystemMonitorService : public QObject
{
    Q_OBJECT
public:
    static SystemMonitorService* instance();
    static SystemMonitorService* create(QObject* parent = nullptr);
    static void destroy();
    SystemMonitorService(const SystemMonitorService&) = delete;
    SystemMonitorService& operator=(const SystemMonitorService&) = delete;
    Disk checkStorage(const QString& path);

private:
    inline static SystemMonitorService* m_instance {nullptr};
    QTimer* timer;
    CpuTimes previousCpuTimes;
    QList<CpuTimes> previousCoreTimes;
    QMap<QString, DiskStats> m_previousStats;
    qint64 m_previousTime;

private:
    explicit SystemMonitorService(QObject *parent = nullptr);
    QList<CpuTimes> readCpuTimes();
    double countCpuUsage(const CpuTimes& old, const CpuTimes& current);
    void checkCpu();
    void checkMemory();
    void updateDiskStats();
    CpuTimes parseCpuLine(const QString& line);

private slots:
    void timeout();

signals:
    void cpuUsage(const QList<double>& usage);
    void memoryUsage(quint64 used, quint64 free);
    void diskStatsChanged(const QMap<QString, DiskStats>& stats);
};

#endif // SYSTEMMONITORSERVICE_H

#ifndef SYSTEMMONITORSERVICE_H
#define SYSTEMMONITORSERVICE_H

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

class SystemMonitorService : public QObject
{
    Q_OBJECT
public:
    static SystemMonitorService* instance();
    static SystemMonitorService* create(QObject* parent = nullptr);
    static void destroy();
    SystemMonitorService(const SystemMonitorService&) = delete;
    SystemMonitorService& operator=(const SystemMonitorService&) = delete;

private:
    inline static SystemMonitorService* m_instance {nullptr};
    QTimer* timer;
    CpuTimes previousCpuTimes;

private:
    explicit SystemMonitorService(QObject *parent = nullptr);
    CpuTimes readCpuTimes();
    double countCpuUsage(const CpuTimes& old, const CpuTimes& current);
    void checkCpu();

private slots:
    void timeout();

signals:
    void cpuUsage(double val);
};

#endif // SYSTEMMONITORSERVICE_H

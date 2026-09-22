#ifndef SWAYSERVICE_H
#define SWAYSERVICE_H

#include <QObject>
#include <qlocalsocket.h>

class SwayService : public QObject
{
    Q_OBJECT
public:
    static SwayService* instance();
    static SwayService* create(QObject* parent = nullptr);
    static void destroy();
    SwayService(const SwayService&) = delete;
    SwayService& operator=(const SwayService&) = delete;

private:
    explicit SwayService(QObject *parent = nullptr);
    QByteArray sendIpcCommand(const QByteArray& command);

private slots:
    void onReadReady();

private:
    QLocalSocket* socket2 {nullptr};
    inline static SwayService* m_instance {nullptr};

signals:
};

#endif // SWAYSERVICE_H

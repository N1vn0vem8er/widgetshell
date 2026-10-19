#ifndef IPCSERVERSERVICE_H
#define IPCSERVERSERVICE_H

#include <QLocalServer>
#include <QObject>

class IPCServerService : public QObject
{
    Q_OBJECT
public:
    static IPCServerService* instance();
    static IPCServerService* create(QObject* parent = nullptr);
    static void destroy();
    IPCServerService(const IPCServerService&) = delete;
    IPCServerService& operator=(const IPCServerService&) = delete;

private:
    explicit IPCServerService(QObject *parent = nullptr);

private:
    inline static IPCServerService* m_instance = nullptr;
    QLocalServer* server;

private slots:
    void handleConnections();

signals:
};

#endif // IPCSERVERSERVICE_H

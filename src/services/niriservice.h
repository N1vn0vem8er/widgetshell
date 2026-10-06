#ifndef NIRISERVICE_H
#define NIRISERVICE_H

#include <QJsonObject>
#include <QObject>
#include <qlocalsocket.h>

class NiriService : public QObject
{
    Q_OBJECT
public:
    static NiriService* instance();
    static NiriService* create(QObject* parent = nullptr);
    static void destroy();
    NiriService(const NiriService&) = delete;
    NiriService& operator=(const NiriService&) = delete;

private:
    QLocalSocket* commandSocket {nullptr};
    QLocalSocket* eventSocket {nullptr};
    inline static NiriService* m_instance {nullptr};
    QByteArray m_commandReadBuffer;
    QByteArray m_eventReadBuffer;

private:
    explicit NiriService(QObject *parent = nullptr);
    ~NiriService();
    void sendIpcCommand(const QByteArray& command);
    void processCommandResponse(const QJsonObject& object);

private slots:
    void onCommandReadReady();
    void onEventReadReady();

signals:
};

#endif // NIRISERVICE_H

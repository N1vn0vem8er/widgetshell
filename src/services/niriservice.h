#ifndef NIRISERVICE_H
#define NIRISERVICE_H

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
    QLocalSocket* socket {nullptr};
    inline static NiriService* m_instance {nullptr};

private:
    explicit NiriService(QObject *parent = nullptr);
    ~NiriService();
    void sendIpcCommand(const QByteArray& command);

private slots:
    void onReadReady();

signals:
};

#endif // NIRISERVICE_H

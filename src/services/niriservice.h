#ifndef NIRISERVICE_H
#define NIRISERVICE_H

#include <QObject>

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
    inline static NiriService* m_instance {nullptr};

private:
    explicit NiriService(QObject *parent = nullptr);
    QByteArray sendIpcCommand(const QByteArray& command);

private slots:
    void onReadReady();

signals:
};

#endif // NIRISERVICE_H

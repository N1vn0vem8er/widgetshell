#ifndef MPRISSERVICE_H
#define MPRISSERVICE_H

#include <QObject>

class MprisService : public QObject
{
    Q_OBJECT
public:
    static MprisService* instance();
    static MprisService* create(QObject* parent = nullptr);
    static void destroy();
    MprisService(const MprisService&) = delete;
    MprisService& operator=(const MprisService&) = delete;

private:
    explicit MprisService(QObject *parent = nullptr);

private:
    inline static MprisService* m_instance {nullptr};

signals:
};

#endif // MPRISSERVICE_H

#ifndef AUDIOSERVICE_H
#define AUDIOSERVICE_H

#include <QObject>

class AudioService : public QObject
{
    Q_OBJECT
public:
    explicit AudioService(QObject *parent = nullptr);

signals:
};

#endif // AUDIOSERVICE_H

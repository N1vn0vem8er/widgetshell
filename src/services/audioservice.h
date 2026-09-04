#ifndef AUDIOSERVICE_H
#define AUDIOSERVICE_H

#include <QObject>

class AudioService : public QObject
{
    Q_OBJECT
public:
    static AudioService* instance();
    static AudioService* create(QObject* parent = nullptr);
    static void destroy();
    AudioService(const AudioService&) = delete;
    AudioService& operator=(const AudioService&) = delete;

private:
    explicit AudioService(QObject *parent = nullptr);
    ~AudioService();

private:
    inline static AudioService* m_instance;

signals:
    void volumeChanged(float val);
    void muteChanged(bool val);
};

#endif // AUDIOSERVICE_H

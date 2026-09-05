#ifndef AUDIOSERVICE_H
#define AUDIOSERVICE_H

#include <QMutex>
#include <QObject>
#include <QVector>
#include <QString>
#include <pulse/pulseaudio.h>
#include <pulse/thread-mainloop.h>

struct AudioDevice {
    uint32_t id;
    QString name;
    QString description;
};

class AudioService : public QObject
{
    Q_OBJECT
public:
    static AudioService* instance();
    static AudioService* create(QObject* parent = nullptr);
    static void destroy();
    void setVolume(float volume);
    void setMute(bool mute);
    void setDefaultOutput(const QString &nodeName);
    QVector<AudioDevice> getOutputDevices();
    AudioService(const AudioService&) = delete;
    AudioService& operator=(const AudioService&) = delete;

private:
    explicit AudioService(QObject *parent = nullptr);
    ~AudioService();
    void updateDevicesAndVolume();
    static void contextStateCallback(pa_context* c, void* userdata);
    static void subscribeCallback(pa_context* c, pa_subscription_event_type_t t, uint32_t index, void* userdata);
    static void serverInfoCallback(pa_context* c, const pa_server_info* i, void* userdata);
    static void sinkInfoCallback(pa_context* c, const pa_sink_info* i, int eol, void* userdata);

private:
    inline static AudioService* m_instance = nullptr;
    pa_threaded_mainloop *m_mainloop = nullptr;
    pa_context *m_context = nullptr;
    mutable QMutex m_mutex;
    QVector<AudioDevice> m_devices;
    QVector<AudioDevice> m_tempDevices;
    QString m_defaultSinkName;
    uint8_t m_defaultSinkChannels {2};
    float m_currentVolume {-1.0f};
    bool m_currentMute {false};

signals:
    void volumeChanged(float val);
    void muteChanged(bool val);
    void devicesChanged();
};

#endif // AUDIOSERVICE_H
#include "audioservice.h"
#include <QMutexLocker>
#include <QDebug>

AudioService *AudioService::instance()
{
    return m_instance;
}

AudioService *AudioService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new AudioService(parent);
    return m_instance;
}

void AudioService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

AudioService::AudioService(QObject *parent)
    : QObject{parent}
{
    m_mainloop = pa_threaded_mainloop_new();
    if(!m_mainloop)
    {
        qWarning() << "Can't create pa_threaded_mainloop";
        return;
    }

    pa_mainloop_api* api = pa_threaded_mainloop_get_api(m_mainloop);

    m_context = pa_context_new(api, "widgetshell");
    if(!m_context)
    {
        qWarning() << "Can't create context";
        pa_threaded_mainloop_free(m_mainloop);
        m_mainloop = nullptr;
        return;
    }
    pa_context_set_state_callback(m_context, contextStateCallback, this);
    pa_context_connect(m_context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
    pa_threaded_mainloop_start(m_mainloop);
}

AudioService::~AudioService()
{
    if(m_mainloop)
        pa_threaded_mainloop_stop(m_mainloop);

    if(m_context)
    {
        pa_context_disconnect(m_context);
        pa_context_unref(m_context);
    }

    if(m_mainloop)
        pa_threaded_mainloop_free(m_mainloop);
}

void AudioService::setVolume(float volume)
{
    if(!m_context || m_defaultSinkName.isEmpty())
        return;

    pa_volume_t paVolume = (pa_volume_t)(qBound(0.0f, volume, 1.5f) * PA_VOLUME_NORM);
    pa_cvolume cvol;
    pa_cvolume_set(&cvol, m_defaultSinkChannels, paVolume);
    pa_threaded_mainloop_lock(m_mainloop);
    pa_operation* op = pa_context_set_sink_volume_by_name(m_context, m_defaultSinkName.toUtf8().constData(), &cvol, nullptr, nullptr);

    if(op)
        pa_operation_unref(op);
    pa_threaded_mainloop_unlock(m_mainloop);
}

void AudioService::setMute(bool mute)
{
    if(!m_context || m_defaultSinkName.isEmpty())
        return;

    pa_threaded_mainloop_lock(m_mainloop);
    pa_operation* op = pa_context_set_sink_mute_by_name(m_context, m_defaultSinkName.toUtf8().constData(), mute ? 1 : 0, nullptr, nullptr);

    if(op)
        pa_operation_unref(op);

    pa_threaded_mainloop_unlock(m_mainloop);
}

void AudioService::setDefaultOutput(const QString &nodeName)
{
    if(!m_context || nodeName.isEmpty())
        return;

    pa_threaded_mainloop_lock(m_mainloop);
    pa_operation* op = pa_context_set_default_sink(m_context, nodeName.toUtf8().constData(), nullptr, nullptr);
    if(op)
        pa_operation_unref(op);

    pa_threaded_mainloop_unlock(m_mainloop);
}

QVector<AudioDevice> AudioService::getOutputDevices()
{
    QMutexLocker locker(&m_mutex);
    return m_devices;
}

void AudioService::contextStateCallback(pa_context *c, void *userdata)
{
    auto* self = static_cast<AudioService*>(userdata);

    switch (pa_context_get_state(c)) {
    case PA_CONTEXT_READY: {
        qDebug() << "PulseAudio context ready";
        pa_context_set_subscribe_callback(c, subscribeCallback, self);
        pa_operation* op = pa_context_subscribe(c, (pa_subscription_mask_t)(PA_SUBSCRIPTION_MASK_SINK | PA_SUBSCRIPTION_MASK_SERVER), nullptr, nullptr);
        if(op)
            pa_operation_unref(op);

        self->updateDevicesAndVolume();
        break;
    }
    case PA_CONTEXT_FAILED:
    case PA_CONTEXT_TERMINATED:
        qWarning() << "Disconnected";
        break;
    default:
        break;
    }
}

void AudioService::subscribeCallback(pa_context *c, pa_subscription_event_type_t t, uint32_t index, void *userdata)
{
    Q_UNUSED(index);
    auto* self = static_cast<AudioService*>(userdata);
    int facility = t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK;

    if(facility == PA_SUBSCRIPTION_EVENT_SINK || facility == PA_SUBSCRIPTION_EVENT_SERVER)
        self->updateDevicesAndVolume();
}

void AudioService::updateDevicesAndVolume()
{
    pa_operation* op = pa_context_get_server_info(m_context, serverInfoCallback, this);
    if(op)
        pa_operation_unref(op);
}

void AudioService::serverInfoCallback(pa_context *c, const pa_server_info *i, void *userdata)
{
    auto* self = static_cast<AudioService*>(userdata);
    if(i && i->default_sink_name)
    {
        self->m_defaultSinkName = QString::fromUtf8(i->default_sink_name);
        self->m_tempDevices.clear();
        pa_operation* op = pa_context_get_sink_info_list(c, sinkInfoCallback, self);
        if(op)
            pa_operation_unref(op);
    }
}

void AudioService::sinkInfoCallback(pa_context *c, const pa_sink_info *i, int eol, void *userdata)
{
    Q_UNUSED(c);
    auto* self = static_cast<AudioService*>(userdata);

    if(eol > 0)
    {
        QMutexLocker locker(&self->m_mutex);
        self->m_devices = self->m_tempDevices;
        self->m_tempDevices.clear();
        emit self->devicesChanged();
        return;
    }

    if(eol < 0 || !i)
        return;

    AudioDevice dev;
    dev.id = i->index;
    dev.name = QString::fromUtf8(i->name);
    dev.description = QString::fromUtf8(i->description);
    self->m_tempDevices.append(dev);

    if(dev.name == self->m_defaultSinkName)
    {
        self->m_defaultSinkChannels = i->channel_map.channels;
        float vol = static_cast<float>(pa_cvolume_avg(&i->volume)) / PA_VOLUME_NORM;
        bool mute = i->mute > 0;

        if(!qFuzzyCompare(self->m_currentVolume, vol))
        {
            self->m_currentVolume = vol;
            emit self->volumeChanged(vol);
        }

        if(self->m_currentMute != mute)
        {
            self->m_currentMute = mute;
            emit self->muteChanged(mute);
        }
    }
}

bool AudioService::currentMute() const
{
    return m_currentMute;
}

float AudioService::currentVolume() const
{
    return m_currentVolume;
}
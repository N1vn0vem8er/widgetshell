#include "audioservice.h"

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
{}

AudioService::~AudioService()
{

}

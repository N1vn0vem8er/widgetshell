#include "audiovolumecontrolbarcomponent.h"

#include <QWheelEvent>

AudioVolumeControlBarComponent::AudioVolumeControlBarComponent(QWidget *parent)
    : QPushButton{parent}
{
    service = AudioService::instance();

    volume = service->currentVolume();
    muted = service->currentMute();

    connect(service, &AudioService::volumeChanged, this, &AudioVolumeControlBarComponent::volumeChanged);
    connect(service, &AudioService::muteChanged, this, &AudioVolumeControlBarComponent::muteChanged);
    connect(this, &QPushButton::clicked, this, &AudioVolumeControlBarComponent::changeMute);

    volumeChanged(volume);
    muteChanged(muted);
}

void AudioVolumeControlBarComponent::updateVolumeIcon()
{
    if(muted)
        setIcon(QIcon::fromTheme("audio-volume-muted"));
    else if(volume <= 0.33)
        setIcon(QIcon::fromTheme("audio-volume-low"));
    else if(volume > 0.33 && volume <= 0.66)
        setIcon(QIcon::fromTheme("audio-volume-medium"));
    else if(volume > 0.66)
        setIcon(QIcon::fromTheme("audio-volume-high"));
}

void AudioVolumeControlBarComponent::volumeChanged(float val)
{
    volume = val;
    setText(QString("%1%").arg(qRound(volume * 100)));
    updateVolumeIcon();
}

void AudioVolumeControlBarComponent::muteChanged(bool val)
{
    muted = val;
    updateVolumeIcon();
}

void AudioVolumeControlBarComponent::changeMute()
{
    service->setMute(!muted);
}

void AudioVolumeControlBarComponent::wheelEvent(QWheelEvent *event)
{
    const int delta = event->angleDelta().y();
    if(delta == 0)
        return;

    float newVolume = volume;
    if(delta > 0)
        newVolume += 0.05f;
    else
        newVolume -= 0.05f;

    newVolume = qBound(0.0f, newVolume, 1.0f);

    service->setVolume(newVolume);
    event->accept();
}

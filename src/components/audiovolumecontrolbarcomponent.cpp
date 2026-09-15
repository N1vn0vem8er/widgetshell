#include "audiovolumecontrolbarcomponent.h"

AudioVolumeControlBarComponent::AudioVolumeControlBarComponent(QWidget *parent)
    : QWidget{parent}
{
    service = AudioService::instance();

    volume = service->currentVolume();
    muted = service->currentMute();
    audioButton = new QPushButton(this);
    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(audioButton);

    connect(service, &AudioService::volumeChanged, this, &AudioVolumeControlBarComponent::volumeChanged);
    connect(service, &AudioService::muteChanged, this, &AudioVolumeControlBarComponent::muteChanged);

    volumeChanged(volume);
    muteChanged(muted);
}

void AudioVolumeControlBarComponent::updateVolumeIcon()
{
    if(muted)
        audioButton->setIcon(QIcon::fromTheme("audio-volume-muted"));
    else if(volume <= 0.33)
        audioButton->setIcon(QIcon::fromTheme("audio-volume-low"));
    else if(volume > 0.33 && volume <= 0.66)
        audioButton->setIcon(QIcon::fromTheme("audio-volume-medium"));
    else if(volume > 0.66)
        audioButton->setIcon(QIcon::fromTheme("audio-volume-high"));
}

void AudioVolumeControlBarComponent::volumeChanged(float val)
{
    volume = val;
    audioButton->setText(QString("%1%").arg(qRound(volume * 100)));
    updateVolumeIcon();
}

void AudioVolumeControlBarComponent::muteChanged(bool val)
{
    muted = val;
    updateVolumeIcon();
}

void AudioVolumeControlBarComponent::changeVolume(int val)
{
    if(val < 0 || val > 100)
        return;
    service->setVolume(static_cast<float>(val) / 100.0f);
}

void AudioVolumeControlBarComponent::changeMute()
{
    service->setMute(!muted);
}

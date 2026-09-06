#include "audiovolumecontrolcomponent.h"

AudioVolumeControlComponent::AudioVolumeControlComponent(QWidget *parent)
    : QWidget{parent}
{
    service = AudioService::instance();
    volume = service->currentVolume();
    muted = service->currentMute();
    connect(service, &AudioService::volumeChanged, this, &AudioVolumeControlComponent::volumeChanged);
    connect(service, &AudioService::muteChanged, this, &AudioVolumeControlComponent::muteChanged);

    mainLayout = new QHBoxLayout(this);

    muteButton = new QPushButton(this);
    muteButton->setFlat(true);
    connect(muteButton, &QPushButton::clicked, this, &AudioVolumeControlComponent::changeMute);
    mainLayout->addWidget(muteButton);

    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(0);
    slider->setSingleStep(1);
    slider->setPageStep(5);
    connect(slider, &QSlider::valueChanged, this, &AudioVolumeControlComponent::changeVolume);
    mainLayout->addWidget(slider);

    volumeLabel = new QLabel("0%", this);
    mainLayout->addWidget(volumeLabel);

    mainLayout->setStretch(1, 1);
    volumeChanged(volume);
    muteChanged(muted);
}

void AudioVolumeControlComponent::updateVolumeIcon()
{
    if(muted)
        muteButton->setIcon(QIcon::fromTheme("audio-volume-muted"));
    else if(volume <= 0.33)
        muteButton->setIcon(QIcon::fromTheme("audio-volume-low"));
    else if(volume > 0.33 && volume <= 0.66)
        muteButton->setIcon(QIcon::fromTheme("audio-volume-medium"));
    else if(volume > 0.66)
        muteButton->setIcon(QIcon::fromTheme("audio-volume-high"));
}

void AudioVolumeControlComponent::volumeChanged(float val)
{
    volume = val;
    volumeLabel->setText(QString("%1%").arg(qRound(volume * 100)));
    slider->blockSignals(true);
    slider->setValue(qRound(volume * 100.0f));
    slider->blockSignals(false);
    updateVolumeIcon();
}

void AudioVolumeControlComponent::muteChanged(bool val)
{
    muted = val;
    updateVolumeIcon();
}

void AudioVolumeControlComponent::changeVolume(int val)
{
    if(val < 0 || val > 100)
        return;
    service->setVolume(static_cast<float>(val) / 100.0f);
}

void AudioVolumeControlComponent::changeMute()
{
    service->setMute(!muted);
}

#include "audiovolumecontrolcomponent.h"

AudioVolumeControlComponent::AudioVolumeControlComponent(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QHBoxLayout(this);

    muteButton = new QPushButton(this);
    muteButton->setFlat(true);
    mainLayout->addWidget(muteButton);

    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(0);
    mainLayout->addWidget(slider);

    volumeLabel = new QLabel("0%", this);
    mainLayout->addWidget(volumeLabel);

    mainLayout->setStretch(1, 1);
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
    volumeLabel->setText(QString("%1%").arg(volume));
    slider->setValue(volume * 100);
    updateVolumeIcon();
}

void AudioVolumeControlComponent::muteChanged(bool val)
{
    muted = val;
    updateVolumeIcon();
}

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
    mainLayout->addWidget(slider);

    volumeLabel = new QLabel("0%", this);
    mainLayout->addWidget(volumeLabel);

    mainLayout->setStretch(1, 1);
}

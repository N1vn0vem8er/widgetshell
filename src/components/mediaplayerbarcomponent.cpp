#include "mediaplayerbarcomponent.h"
#include <qboxlayout.h>

MediaPlayerBarComponent::MediaPlayerBarComponent(QWidget *parent)
    : QWidget{parent}
{
    service = MprisService::instance();

    trackNameLabel = new QLabel(this);
    auto layout = new QHBoxLayout(this);
    layout->addWidget(trackNameLabel);

    connect(service, &MprisService::trackChanged, this, &MediaPlayerBarComponent::trackChanged);
}

void MediaPlayerBarComponent::trackChanged(const Track &track)
{
    trackNameLabel->setText(track.title);
}

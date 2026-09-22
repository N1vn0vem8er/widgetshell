#include "mediaplayerbarcomponent.h"
#include <qboxlayout.h>

MediaPlayerBarComponent::MediaPlayerBarComponent(QWidget *parent)
    : QWidget{parent}
{
    service = MprisService::instance();

    trackNameLabel = new QLabel(this);
    auto layout = new QHBoxLayout(this);
    layout->addWidget(trackNameLabel);
    nextButton = new QPushButton(QIcon::fromTheme("media-skip-forward"), "", this);
    previousButton = new QPushButton(QIcon::fromTheme("media-skip-backward"), "", this);
    playPauseButton = new QPushButton(QIcon::fromTheme("media-playback-start"), "", this);
    layout->addWidget(previousButton);
    layout->addWidget(playPauseButton);
    layout->addWidget(nextButton);

    connect(service, &MprisService::trackChanged, this, &MediaPlayerBarComponent::trackChanged);
    connect(service, &MprisService::playbackStatusChanged, this, &MediaPlayerBarComponent::statusChanged);
}

void MediaPlayerBarComponent::trackChanged(const Track &track)
{
    trackNameLabel->setText(track.title);
}

void MediaPlayerBarComponent::playPause()
{
    if(service->isPaused())
        service->play();
    else
        service->pause();
}

void MediaPlayerBarComponent::next()
{
    service->next();
}

void MediaPlayerBarComponent::previous()
{
    service->previous();
}

void MediaPlayerBarComponent::statusChanged(const QString&)
{
    if(service->isPaused())
        playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
    else
        playPauseButton->setIcon(QIcon::fromTheme("media-playback-pause"));
}

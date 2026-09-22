#ifndef MEDIAPLAYERBARCOMPONENT_H
#define MEDIAPLAYERBARCOMPONENT_H

#include "services/mprisservice.h"
#include <QWidget>
#include <qlabel.h>
#include <qpushbutton.h>

class MediaPlayerBarComponent : public QWidget
{
    Q_OBJECT
public:
    explicit MediaPlayerBarComponent(QWidget *parent = nullptr);

private:
    MprisService* service;
    QLabel* trackNameLabel;
    QPushButton* playPauseButton;
    QPushButton* nextButton;
    QPushButton* previousButton;

private slots:
    void trackChanged(const Track& track);
    void playPause();
    void next();
    void previous();
    void statusChanged(const QString&);

signals:
};

#endif // MEDIAPLAYERBARCOMPONENT_H

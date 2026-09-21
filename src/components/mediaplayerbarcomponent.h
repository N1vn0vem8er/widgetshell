#ifndef MEDIAPLAYERBARCOMPONENT_H
#define MEDIAPLAYERBARCOMPONENT_H

#include "services/mprisservice.h"
#include <QWidget>
#include <qlabel.h>

class MediaPlayerBarComponent : public QWidget
{
    Q_OBJECT
public:
    explicit MediaPlayerBarComponent(QWidget *parent = nullptr);

private:
    MprisService* service;
    QLabel* trackNameLabel;

private slots:
    void trackChanged(const Track& track);

signals:
};

#endif // MEDIAPLAYERBARCOMPONENT_H

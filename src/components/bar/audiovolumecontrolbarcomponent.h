#ifndef AUDIOVOLUMECONTROLBARCOMPONENT_H
#define AUDIOVOLUMECONTROLBARCOMPONENT_H

#include "services/audioservice.h"
#include <QWidget>
#include <qboxlayout.h>
#include <qpushbutton.h>

class AudioVolumeControlBarComponent : public QPushButton
{
    Q_OBJECT
public:
    explicit AudioVolumeControlBarComponent(QWidget *parent = nullptr);

private:
    AudioService* service;
    bool muted;
    float volume;

private:
    void updateVolumeIcon();

private slots:
    void volumeChanged(float val);
    void muteChanged(bool val);
    void changeMute();

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // AUDIOVOLUMECONTROLBARCOMPONENT_H

#ifndef AUDIOVOLUMECONTROLBARCOMPONENT_H
#define AUDIOVOLUMECONTROLBARCOMPONENT_H

#include "services/audioservice.h"
#include <QWidget>
#include <qboxlayout.h>
#include <qpushbutton.h>

class AudioVolumeControlBarComponent : public QWidget
{
    Q_OBJECT
public:
    explicit AudioVolumeControlBarComponent(QWidget *parent = nullptr);

private:
    AudioService* service;
    QPushButton* audioButton;
    QHBoxLayout* mainLayout;
    bool muted;
    float volume;

private:
    void updateVolumeIcon();

private slots:
    void volumeChanged(float val);
    void muteChanged(bool val);
    void changeVolume(int val);
    void changeMute();

signals:
};

#endif // AUDIOVOLUMECONTROLBARCOMPONENT_H

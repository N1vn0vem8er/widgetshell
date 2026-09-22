#ifndef AUDIOVOLUMECONTROLCOMPONENT_H
#define AUDIOVOLUMECONTROLCOMPONENT_H

#include "services/audioservice.h"
#include <QWidget>
#include <QSlider>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QComboBox>

class AudioVolumeControlComponent : public QWidget
{
    Q_OBJECT
public:
    explicit AudioVolumeControlComponent(QWidget *parent = nullptr);

private:
    QComboBox* outputComboBox;
    QSlider* slider;
    QPushButton* muteButton;
    QVBoxLayout* mainLayout;
    QHBoxLayout* volumeControlLayout;
    QLabel* volumeLabel;
    bool muted;
    float volume;
    AudioService* service;

private:
    void updateVolumeIcon();

private slots:
    void volumeChanged(float val);
    void muteChanged(bool val);
    void changeVolume(int val);
    void changeMute();
};

#endif // AUDIOVOLUMECONTROLCOMPONENT_H

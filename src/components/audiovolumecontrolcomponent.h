#ifndef AUDIOVOLUMECONTROLCOMPONENT_H
#define AUDIOVOLUMECONTROLCOMPONENT_H

#include <QWidget>
#include <QSlider>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

class AudioVolumeControlComponent : public QWidget
{
    Q_OBJECT
public:
    explicit AudioVolumeControlComponent(QWidget *parent = nullptr);

private:
    QSlider* slider;
    QPushButton* muteButton;
    QHBoxLayout* mainLayout;
    QLabel* volumeLabel;
    bool muted;
    float volume;

private:
    void updateVolumeIcon();

private slots:
    void volumeChanged(float val);
    void muteChanged(bool val);

signals:
};

#endif // AUDIOVOLUMECONTROLCOMPONENT_H

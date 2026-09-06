#ifndef CONTROLMENU_H
#define CONTROLMENU_H

#include "components/audiovolumecontrolcomponent.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QCalendarWidget>

class ControlMenu : public QWidget
{
    Q_OBJECT
public:
    explicit ControlMenu(QWidget *parent = nullptr);

private:
    QVBoxLayout* mainLayout;
    QPushButton* shutdownButton;
    QPushButton* logoutButton;
    QPushButton* sleepButton;
    QPushButton* rebootButton;
    QCalendarWidget* calendar;
    AudioVolumeControlComponent* audioVolumeControl;

private slots:
    void onShutdown();
    void onLogout();
    void onSleep();
    void onReboot();

signals:
};

#endif // CONTROLMENU_H

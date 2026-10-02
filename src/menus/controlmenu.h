#ifndef CONTROLMENU_H
#define CONTROLMENU_H

#include "components/menus/audiovolumecontrolcomponent.h"
#include "components/menus/notificationhistorycomponent.h"
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
    QCalendarWidget* calendar;
    AudioVolumeControlComponent* audioVolumeControl;
    NotificationHistoryComponent* notificationHistory;

signals:
};

#endif // CONTROLMENU_H

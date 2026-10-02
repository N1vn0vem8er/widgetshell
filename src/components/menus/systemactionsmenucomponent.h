#ifndef SYSTEMACTIONSMENUCOMPONENT_H
#define SYSTEMACTIONSMENUCOMPONENT_H

#include <QWidget>
#include <qpushbutton.h>

class SystemActionsMenuComponent : public QWidget
{
    Q_OBJECT
public:
    explicit SystemActionsMenuComponent(QWidget *parent = nullptr);

private:
    QPushButton* shutdownButton;
    QPushButton* logoutButton;
    QPushButton* sleepButton;
    QPushButton* rebootButton;

private slots:
    void onShutdown();
    void onLogout();
    void onSleep();
    void onReboot();

signals:
};

#endif // SYSTEMACTIONSMENUCOMPONENT_H

#ifndef CONTROLMENU_H
#define CONTROLMENU_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

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

private slots:
    void onShutdown();
    void onLogout();
    void onSleep();
    void onReboot();

signals:
};

#endif // CONTROLMENU_H

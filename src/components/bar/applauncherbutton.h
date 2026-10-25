#ifndef APPLAUNCHERBUTTON_H
#define APPLAUNCHERBUTTON_H

#include "services/desktopappsservice.h"
#include <QPushButton>

class AppLauncherButton : public QPushButton
{
    Q_OBJECT
public:
    AppLauncherButton(const QString& icon, const QString& text, QWidget* parent = nullptr);

private:
    DesktopAppsService* service;
};

#endif // APPLAUNCHERBUTTON_H

#include "applauncherbutton.h"

AppLauncherButton::AppLauncherButton(const QString &icon, const QString &text, QWidget *parent)
{
    setText(text);
    setIcon(QIcon::fromTheme(icon));
    service = DesktopAppsService::instance();
    connect(this, &QPushButton::clicked, service, &DesktopAppsService::showLauncher);
}

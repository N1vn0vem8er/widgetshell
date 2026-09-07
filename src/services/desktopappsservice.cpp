#include "desktopappsservice.h"
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <qregularexpression.h>

DesktopAppsService *DesktopAppsService::instance()
{
    return m_instance;
}

DesktopAppsService *DesktopAppsService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new DesktopAppsService(parent);
    return m_instance;
}

void DesktopAppsService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

DesktopAppsService::DesktopAppsService(QObject *parent)
    : QObject{parent}
{
    scanForApps();
}

DesktopAppsService::~DesktopAppsService()
{

}

QList<DesktopApp> DesktopAppsService::desktopApps() const
{
    return m_desktopApps;
}

void DesktopAppsService::scanForApps()
{
    m_desktopApps.clear();
    const QStringList dirs{
        QDir::homePath() + "/.local/share/applications",
        QDir::homePath() + "/.local/share/flatpak/exports/share/applications",
        "/usr/local/share/applications",
        "/usr/share/applications",
        "/var/lib/flatpak/exports/share/applications",
        "/var/lib/snapd/desktop/applications"
    };

    QSet<QString> processedAppIds;
    static const QRegularExpression fieldCodexRegex(R"(\s%[fFuUiIkK])");
    static const QRegularExpression desktopRegex("\\.desktop$");

    for(const QString& dirPath : dirs)
    {
        QDir dir(dirPath);
        if(!dir.exists())
            continue;

        QDirIterator it(dirPath, {"*.desktop"}, QDir::Files, QDirIterator::Subdirectories);
        while(it.hasNext())
        {
            QString filePath = it.next();
            if(processedAppIds.contains(filePath))
                continue;

            QSettings desktopFile(filePath, QSettings::IniFormat);
            desktopFile.beginGroup("Desktop Entry");

            DesktopApp app;
            app.fileName = QFileInfo(filePath).fileName().remove(desktopRegex);
            app.name = desktopFile.value("Name", "").toString();
            app.type = desktopFile.value("Type", "").toString();
            app.genericName = desktopFile.value("GenericName", "").toString();
            app.icon = desktopFile.value("Icon", "").toString();
            QString exec = desktopFile.value("Exec", "").toString();
            exec.replace("%c", app.name);
            exec.remove(fieldCodexRegex);
            app.exec = exec;
            app.startupWMClass = desktopFile.value("StartupWMClass", "").toString();
            m_desktopApps.append(app);
            processedAppIds.insert(filePath);
        }
    }
}

#ifndef DESKTOPAPPSSERVICE_H
#define DESKTOPAPPSSERVICE_H

#include <QObject>

struct DesktopApp {
    QString type;
    QString name;
    QString genericName;
    QString comment;
    QString icon;
    std::optional<bool> noDisplay;
    std::optional<bool> hidden;
    QList<QString> onlyShowIn;
    QList<QString> notShowIn;
    std::optional<bool> dbusActivatable;
    QString tryExec;
    QString exec;
    QString path;
    std::optional<bool> terminal;
    QList<QString> actions;
    QList<QString> mimeType;
    QList<QString> categories;
    QList<QString> implements;
    QList<QString> keywords;
    std::optional<bool> startupNotify;
    QString startupWMClass;
    QString url;
    std::optional<bool> prefersNonDefaultGPU;
    std::optional<bool> singleMainWindow;
};

class DesktopAppsService : public QObject
{
    Q_OBJECT
public:
    explicit DesktopAppsService(QObject *parent = nullptr);
    ~DesktopAppsService();

private:
    QList<DesktopApp> m_desktopApps;

public slots:
    void scanForApps();
signals:
};

#endif // DESKTOPAPPSSERVICE_H

#ifndef HYPRLANDSERVICE_H
#define HYPRLANDSERVICE_H

#include <QJsonObject>
#include <QLocalSocket>
#include <QObject>
#include <qjsonarray.h>
#include <qpoint.h>
#include <qsize.h>

struct HyprlandWorkspaceR {
    int id;
    QString name;
};

struct HyprlandWorkspace {
    int id;
    QString name;
    int windowsCount;
    bool hasFullscreen;
};

struct HyprlandClient {
    quint64 address;
    bool mapped;
    bool hidden;
    bool visible;
    bool acceptsInput;
    QPoint at;
    QSize size;
    HyprlandWorkspaceR workspace;
    bool floating;
    int monitor;
    QString appClass;
    QString title;
    QString initialClass;
    QString initialTitle;
    int pid;
    bool xwayland;
    bool pinned;
    bool pinFullscreened;
    int fullscreen;
    int fullscreenClient;
    QString fullscreenHandler;
    bool allowedOverFullscreen;
    bool tearingHint;
    long stableId;

    static HyprlandClient fromJson(const QJsonObject& obj)
    {
        HyprlandClient client;
        client.address = obj.value("address").toString().toULongLong(nullptr, 16);
        client.mapped = obj.value("mapped").toBool();
        client.hidden = obj.value("hidden").toBool();
        client.visible = obj.value("visible").toBool();
        client.acceptsInput = obj.value("acceptsInput").toBool();
        if(const QJsonArray at = obj.value("at").toArray(); at.size() >= 2)
        {
            client.at = QPoint(at[0].toInt(),at[1].toInt());
        }
        if(const QJsonArray size = obj.value("size").toArray(); size.size() >= 2)
        {
            client.size = QSize(size[0].toInt(),size[1].toInt());
        }
        if(const QJsonObject workspace = obj.value("workspace").toObject(); !workspace.isEmpty())
        {
            client.workspace.id = workspace["id"].toInt();
            client.workspace.name = workspace["name"].toString();
        }
        client.floating = obj.value("floating").toBool();
        client.monitor = obj.value("monitor").toInt();
        client.appClass = obj.value("class").toString();
        client.title = obj.value("title").toString();
        client.initialClass = obj.value("initialClass").toString();
        client.initialTitle = obj.value("initialTitle").toString();
        client.pid = obj.value("pid").toInt();
        client.xwayland = obj.value("xwayland").toBool();
        client.pinned = obj.value("pinned").toBool();
        client.pinFullscreened = obj.value("pinFullscreened").toBool();
        client.fullscreen = obj.value("fullscreen").toInt();
        client.fullscreenClient = obj.value("fullscreenClient").toInt();
        client.fullscreenHandler = obj.value("fullscreenHandler").toString();
        client.allowedOverFullscreen = obj.value("allowedOverFullscreen").toBool();
        client.tearingHint = obj.value("tearingHint").toBool();
        client.stableId = obj.value("stableId").toInteger();
        return client;
    }
};

class HyprlandService : public QObject
{
    Q_OBJECT
public:
    static HyprlandService* instance();
    static HyprlandService* create(QObject* parent = nullptr);
    static void destroy();
    QList<HyprlandWorkspace> getWorkspaces();
    void switchWorkspace(int id);
    QList<HyprlandClient> getClients();
    HyprlandWorkspace getWorkspace(int id);
    void focusWindow(quint64 address);
    int getActiveWorkspaceId();
    HyprlandClient getActiveClient();

    HyprlandService(const HyprlandService&) = delete;
    HyprlandService& operator=(const HyprlandService&) = delete;

private:
    explicit HyprlandService(QObject *parent = nullptr);
    ~HyprlandService();

private:
    QLocalSocket* socket2 {nullptr};
    inline static HyprlandService* m_instance {nullptr};
    QString xdgRuntime;
    QString his;

private:
    QByteArray sendIpcCommand(const QByteArray& command);

private slots:
    void onReadReady();

signals:
    void workspaceChanged(int id);
    void workspacesChanged();
    void clientsChanged();
    void openedWindow(quint64 windowAddress, const QString& workspaceName, const QString& windowClass, const QString& windowTitle);
    void closedWindow(quint64 windowAddress);
    void movedWindow(quint64 windowAddress, const QString& workspaceName);
    void movedWindowV2(quint64 windowAddress, const int workspaceId, const QString& workspaceName);
    void activedWindow(const QString& windowClass, const QString& windowTitle);
    void activedWindowV2(quint64 windowAddress);
    void activeWindowEmpty();
    void createdWorkspace(const QString& workspaceName);
    void screenCasted(int state, const QString& owner);
    void screenCastedV2(int state, const QString& owner, const QString& name);
};

#endif // HYPRLANDSERVICE_H

#ifndef HYPRLANDSERVICE_H
#define HYPRLANDSERVICE_H

#include <QLocalSocket>
#include <QObject>

#include "data/hyprlandworkspace.h"
#include "data/hyprlandclient.h"

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
};

#endif // HYPRLANDSERVICE_H

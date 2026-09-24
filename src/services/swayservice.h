#ifndef SWAYSERVICE_H
#define SWAYSERVICE_H

#include <QHash>
#include <QObject>
#include <qlocalsocket.h>

struct SwayWorkspace
{
    int id {0};
    int number {-1};
    QString name;
    QString output;
    bool focused {false};
    bool visible {false};
    bool urgent {false};
};

struct SwayClient
{
    int id {0};
    int pid {-1};
    QString appId;
    QString title;
    QString windowClass;
    QString windowInstance;
    QString workspace;
    QString output;
    bool focused {false};
    bool urgent {false};
};

class SwayService : public QObject
{
    enum IpcType : quint32
    {
        RUN_COMMAND = 0,
        GET_WORKSPACES = 1,
        SUBSCRIBE = 2,
        GET_OUTPUTS = 3,
        GET_TREE = 4,
        GET_MARKS = 5,
        GET_BAR_CONFIG = 6,
        GET_VERSION = 7,
        GET_BINDING_MODES = 8,
        GET_CONFIG = 9,
        SEND_TICK = 10,
        GET_BINDING_STATE = 12,
        EVENT_WORKSPACE = 0x80000000,
        EVENT_OUTPUT = 0x80000001,
        EVENT_MODE = 0x80000002,
        EVENT_WINDOW = 0x80000003,
        EVENT_BARCONFIG_UPDATE = 0x80000004,
        EVENT_BINDING = 0x80000005,
        EVENT_SHUTDOWN = 0x80000006,
        EVENT_TICK = 0x80000007,
        EVENT_BAR_STATE_UPDATE = 0x80000014,
        EVENT_INPUT = 0x80000015
    };

    Q_OBJECT
public:
    static SwayService* instance();
    static SwayService* create(QObject* parent = nullptr);
    static void destroy();
    SwayService(const SwayService&) = delete;
    SwayService& operator=(const SwayService&) = delete;
    QList<SwayWorkspace> workspaces() const;
    void refreshWorkspaces();
    void refreshClients();
    void focusClient(int id);
    void focusWorkspace(const QString& name);
    QList<SwayClient> clients() const;

private:
    explicit SwayService(QObject *parent = nullptr);
    ~SwayService();
    void sendIpcCommand(quint32 type, const QByteArray& payload);
    void subscribeToEvents();
    void updateWorkspaces(const QJsonArray& array);
    SwayWorkspace parseWorkspace(const QJsonObject& object) const;
    SwayClient parseClient(const QJsonObject& object) const;
    void handleWindowEvent(const QJsonObject& event);
    void updateClients(const QJsonObject& tree);
    void collectClients(const QJsonObject& node, QList<SwayClient>& clients, const QString& workspace, const QString& output);

private slots:
    void onReadReady();
    void onConnected();
    void onDisconnected();
    void onError();

private:
    QLocalSocket* socket2 {nullptr};
    inline static SwayService* m_instance {nullptr};
    QByteArray m_readBuffer;
    QList<SwayWorkspace> m_workspaces;
    QList<SwayClient> m_clients;

signals:
    void clientOpened(const SwayClient& client);
    void clientClosed(int id);
    void clientChanged(const SwayClient& client);
    void clientsChanged();
    void workspacesChanged();
    void clientFocused(int id);
};

#endif // SWAYSERVICE_H

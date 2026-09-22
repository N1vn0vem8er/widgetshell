#ifndef SWAYSERVICE_H
#define SWAYSERVICE_H

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
        EVENT_WORKSPACE = 0x80000000
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
    void focusWorkspace(const QString& name);

private:
    explicit SwayService(QObject *parent = nullptr);
    ~SwayService();
    void sendIpcCommand(quint32 type, const QByteArray& payload);
    void subscribeToWorkspaceEvents();
    void updateWorkspaces(const QJsonArray& array);
    SwayWorkspace parseWorkspace(const QJsonObject& object) const;

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

signals:
    void workspacesChanged();
};

#endif // SWAYSERVICE_H

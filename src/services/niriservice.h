#ifndef NIRISERVICE_H
#define NIRISERVICE_H

#include <QJsonObject>
#include <QObject>
#include <qlocalsocket.h>

struct NiriWorkspace {
    quint64 id;
    quint64 idx;
    std::optional<quint64> activeWindowId;
    bool isActive;
    bool isFocused;
    bool isUrgent;
    QString name;
    QString output;
};

struct NiriClient {
    quint64 id;
    QString appId;
    QString title;
    quint64 workspaceId;
    bool isFloating;
    bool isFocused;
    bool isUrgent;
};

class NiriService : public QObject
{
    Q_OBJECT
public:
    static NiriService* instance();
    static NiriService* create(QObject* parent = nullptr);
    static void destroy();
    NiriService(const NiriService&) = delete;
    NiriService& operator=(const NiriService&) = delete;
    void refreshWorkspaces();
    QList<NiriWorkspace> workspaces() const;
    void focusWorkspace(quint64 id);
    void refreshClients();
    QList<NiriClient> clients() const;

private:
    QLocalSocket* commandSocket {nullptr};
    QLocalSocket* eventSocket {nullptr};
    inline static NiriService* m_instance {nullptr};
    QByteArray m_commandReadBuffer;
    QByteArray m_eventReadBuffer;
    QList<NiriWorkspace> m_workspaces;
    QList<NiriClient> m_clients;

private:
    explicit NiriService(QObject *parent = nullptr);
    ~NiriService();
    void sendIpcCommand(const QByteArray& command);
    void processCommandResponse(const QJsonObject& object);
    void processEventResponse(const QJsonObject& object);
    QList<QJsonObject> parseJsonLines(QByteArray& buffer);

private slots:
    void onCommandReadReady();
    void onEventReadReady();

signals:
    void workspaceActivated(quint64 id);
    void workspacesRefreshed();
    void clientsRefreshed();
    void clientFocused(quint64 id);
};

#endif // NIRISERVICE_H

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

private:
    QLocalSocket* commandSocket {nullptr};
    QLocalSocket* eventSocket {nullptr};
    inline static NiriService* m_instance {nullptr};
    QByteArray m_commandReadBuffer;
    QByteArray m_eventReadBuffer;
    QList<NiriWorkspace> m_workspaces;

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
};

#endif // NIRISERVICE_H

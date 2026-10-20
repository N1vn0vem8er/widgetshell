#include "ipcserverservice.h"

#include <QApplication>
#include <QJsonDocument>
#include <QLocalSocket>

IPCServerService *IPCServerService::instance()
{
    return m_instance;
}

IPCServerService *IPCServerService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new IPCServerService(parent);
    return m_instance;
}

void IPCServerService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

IPCServerService::IPCServerService(QObject *parent)
    : QObject{parent}
{
    server = new QLocalServer(this);
    QLocalServer::removeServer("WIDGETSHELL");

    if(!server->listen("WIDGETSHELL"))
    {
        qWarning() << server->errorString();
        return;
    }
    connect(server, &QLocalServer::newConnection, this, &IPCServerService::handleConnections);
}

IPCServerService::~IPCServerService()
{
    server->removeServer("WIDGETSHELL");
}

void IPCServerService::processMessages(QLocalSocket *socket)
{
    auto& buffer = buffers[socket];

    while(true)
    {
        const auto separator = buffer.indexOf('\n');

        if(separator < 0)
            break;

        auto line = buffer.left(separator).trimmed();
        buffer.remove(0, separator + 1);

        if(line.isEmpty())
            continue;

        QJsonParseError error;
        const auto document = QJsonDocument::fromJson(line, &error);

        if(error.error != QJsonParseError::NoError)
        {
            qWarning() << "Invalid JSON:" << error.errorString();
            continue;
        }

        if(!document.isObject())
        {
            qWarning() << "IPC message must be a JSON object";
            continue;
        }

    }
}

void IPCServerService::handleConnections()
{
    while(server->hasPendingConnections())
    {
        QLocalSocket* socket = server->nextPendingConnection();
        if(!socket)
            continue;

        buffers[socket] = QByteArray{};

        connect(socket, &QLocalSocket::readyRead, this, &IPCServerService::handleReadyRead);
        connect(socket, &QLocalSocket::disconnected, this, &IPCServerService::handleDisconnected);

        if(socket->bytesAvailable() > 0)
            handleReadyRead();
    }
}

void IPCServerService::handleReadyRead()
{
    auto* socket = qobject_cast<QLocalSocket*>(sender());
    if(!socket || !buffers.contains(socket))
        return;

    buffers[socket].append(socket->readAll());

    processMessages(socket);
}

void IPCServerService::handleDisconnected()
{
    auto* socket = qobject_cast<QLocalSocket*>(sender());
    if(!socket)
        return;

    buffers.remove(socket);
    socket->deleteLater();
}

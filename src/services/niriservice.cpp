#include "niriservice.h"
#include <qdebug.h>

NiriService *NiriService::instance()
{
    return m_instance;
}

NiriService *NiriService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new NiriService(parent);
    return m_instance;
}

void NiriService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

NiriService::NiriService(QObject *parent)
    : QObject{parent}
{
    socket = new QLocalSocket(this);

    auto path = QString::fromLocal8Bit(qgetenv("NIRI_SOCKET"));
    socket->connectToServer(path);
}

NiriService::~NiriService()
{
    socket->disconnect();
}

void NiriService::sendIpcCommand(const QByteArray &command)
{
    if(socket->state() != QLocalSocket::ConnectedState)
    {
        qWarning() << "Niri IPC socket is not connected";
        return;
    }
    QByteArray message = command;
    if(!message.endsWith('\n'))
        message.append('\n');

    socket->write(message);
}

void NiriService::onReadReady()
{

}

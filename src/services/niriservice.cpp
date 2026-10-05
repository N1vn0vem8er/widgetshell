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
    commandSocket = new QLocalSocket(this);

    auto path = QString::fromLocal8Bit(qgetenv("NIRI_SOCKET"));
    commandSocket->connectToServer(path);
    connect(commandSocket, &QLocalSocket::readyRead, this, &NiriService::onCommandReadReady);

    eventSocket->connectToServer(path);
    connect(eventSocket, &QLocalSocket::connected, this, [this]{
        const QByteArray request = R"({"EventStream":{}})" "\n";
        eventSocket->write(request);
        eventSocket->flush();
    });
    connect(eventSocket, &QLocalSocket::readyRead, this, &NiriService::onEventReadReady);

    eventSocket = new QLocalSocket(this);
}

NiriService::~NiriService()
{
    commandSocket->disconnect();
}

void NiriService::sendIpcCommand(const QByteArray &command)
{
    if(commandSocket->state() != QLocalSocket::ConnectedState)
    {
        qWarning() << "Niri IPC commandSocket is not connected";
        return;
    }
    QByteArray message = command;
    if(!message.endsWith('\n'))
        message.append('\n');

    commandSocket->write(message);
}

void NiriService::onCommandReadReady()
{
    m_commandReadBuffer.append(commandSocket->readAll());

}

void NiriService::onEventReadReady()
{
    m_eventReadBuffer.append(eventSocket->readAll());

}

#include "niriservice.h"
#include <QJsonDocument>
#include <QJsonParseError>
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

    eventSocket = new QLocalSocket(this);
    eventSocket->connectToServer(path);
    connect(eventSocket, &QLocalSocket::connected, this, [this]{
        const QByteArray request = R"({"EventStream":{}})" "\n";
        eventSocket->write(request);
        eventSocket->flush();
    });
    connect(eventSocket, &QLocalSocket::readyRead, this, &NiriService::onEventReadReady);
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
void NiriService::processCommandResponse(const QJsonObject &object)
{

}

void NiriService::onCommandReadReady()
{
    m_commandReadBuffer.append(commandSocket->readAll());
    while(true)
    {
        const qsizetype newline = m_commandReadBuffer.indexOf('\n');

        if(newline < 0)
            return;

        const QByteArray line = m_commandReadBuffer.left(newline);
        m_commandReadBuffer.remove(0, newline + 1);

        if(line.trimmed().isEmpty())
            continue;

        QJsonParseError error;
        const QJsonDocument document = QJsonDocument::fromJson(line, &error);

        if(error.error != QJsonParseError::NoError)
        {
            qWarning() << "Invalid Niri IPC JSON:" << error.errorString() << line;
            continue;
        }

        if(!document.isObject())
        {
            qWarning() << "Niri IPC response is not an object:" << line;
            continue;
        }

        processCommandResponse(document.object());
    }
}

void NiriService::onEventReadReady()
{
    m_eventReadBuffer.append(eventSocket->readAll());

}

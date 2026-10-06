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

void NiriService::processEventResponse(const QJsonObject &object)
{

}

QList<QJsonObject> NiriService::parseJsonLines(QByteArray &buffer)
{
    QList<QJsonObject> result;
    while(true)
    {
        const qsizetype newline = buffer.indexOf('\n');
        if(newline < 0)
            break;

        const QByteArray line = buffer.left(newline);
        buffer.remove(0, newline + 1);

        if(line.trimmed().isEmpty())
            continue;

        QJsonParseError error;
        const QJsonDocument document = QJsonDocument::fromJson(line, &error);

        if(error.error != QJsonParseError::NoError)
        {
            qWarning() << "Invalid Niri JSON:" << error.errorString() << line;
            continue;
        }

        if(document.isObject())
            result.append(document.object());
    }

    return result;
}

void NiriService::onCommandReadReady()
{
    m_commandReadBuffer.append(commandSocket->readAll());
    for(const auto& response : parseJsonLines(m_commandReadBuffer))
    {
        processCommandResponse(response);
    }
}

void NiriService::onEventReadReady()
{
    m_eventReadBuffer.append(eventSocket->readAll());
    for(const auto& event : parseJsonLines(m_eventReadBuffer))
    {
        processEventResponse(event);
    }
}

#include "swayservice.h"
#include <qdebug.h>

SwayService *SwayService::instance()
{
    return m_instance;
}

SwayService *SwayService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new SwayService(parent);
    return m_instance;
}

void SwayService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

SwayService::SwayService(QObject *parent)
    : QObject{parent}
{
    socket2 = new QLocalSocket(this);
    auto path = QString::fromLocal8Bit(qgetenv("SWAYSOCK"));

    if(path.isEmpty())
    {
        qCritical() << "SWAYSOCK not found";
        return;
    }

    connect(socket2, &QLocalSocket::readyRead, this, &SwayService::onReadReady);

    socket2->connectToServer(path);
}

QByteArray SwayService::sendIpcCommand(const QByteArray &command)
{

}

void SwayService::onReadReady()
{

}

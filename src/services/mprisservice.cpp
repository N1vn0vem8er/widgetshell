#include "mprisservice.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

MprisService *MprisService::instance()
{
    return m_instance;
}

MprisService *MprisService::create(QObject *parent)
{
    if(!m_instance)
        m_instance = new MprisService(parent);
    return m_instance;
}

void MprisService::destroy()
{
    if(m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

QStringList MprisService::players() const
{

}

QString MprisService::currentPlayer() const
{

}

Track MprisService::currentTrack() const
{

}

bool MprisService::isPlaying() const
{

}

bool MprisService::isPaused() const
{

}

bool MprisService::isStopped() const
{

}

void MprisService::play()
{

}

void MprisService::pause()
{

}

void MprisService::playPause()
{

}

void MprisService::stop()
{

}

void MprisService::next()
{

}

void MprisService::previous()
{

}

void MprisService::setPlayer(const QString &player)
{

}

MprisService::MprisService(QObject *parent)
    : QObject{parent}
{
    auto dbus = QDBusConnection::sessionBus();
    dbus.connect("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus", "NameOwnerChanged", this, SLOT(onNameOwnerChanged(QString,QString,QString)));
    refreshPlayers();
}

void MprisService::refreshPlayers()
{
    QDBusInterface dbus("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus", QDBusConnection::sessionBus());

    if(!dbus.isValid())
        return;

    const QDBusReply<QStringList> reply = dbus.call("ListNames");

    if (!reply.isValid())
        return;

    QStringList players;

    for(const QString& name : reply.value())
    {
        if(name.startsWith("org.mpris.MediaPlayer2."))
            players.append(name);
    }

    players.sort();

    if (players == m_players)
        return;

    for(const auto& player : m_players)
    {
        unwatchPlayer(player);
    }

    m_players = players;

    for(const auto& player : m_players)
    {
        watchPlayer(player);
    }

    emit playersChanged();

    if(!m_players.contains(m_currentPlayer))
    {
        m_currentPlayer.clear();
        selectPlayer();
    }
}

void MprisService::selectPlayer()
{
    if(m_players.isEmpty())
    {
        if(!m_currentPlayer.isEmpty())
        {
            m_currentPlayer.clear();
            emit playerChanged({});
        }
        return;
    }

    if(!m_currentPlayer.isEmpty() && m_players.contains(m_currentPlayer))
        return;

    setPlayer(m_players.first());
}

bool MprisService::callPlayerMethod(const QString &method)
{
    if(m_currentPlayer.isEmpty())
        return false;

    QDBusInterface player(m_currentPlayer, "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player", QDBusConnection::sessionBus());

    if(!player.isValid())
        return false;

    const QDBusMessage reply = player.call(method);

    return reply.type() != QDBusMessage::ErrorMessage;
}

QVariant MprisService::getProperty(const QString &interfaceName, const QString &propertyName) const
{
    if(m_currentPlayer.isEmpty())
        return {};

    QDBusInterface properties(m_currentPlayer, "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", QDBusConnection::sessionBus());

    if(!properties.isValid())
        return {};

    const QDBusReply<QVariant> reply = properties.call("Get", interfaceName, propertyName);

    if(!reply.isValid())
        return {};

    return reply.value();
}

void MprisService::updateTrack()
{
    Track track;
    const QVariant metadata = getProperty("org.mpris.MediaPlayer2.Player", "Metadata");

    if(!metadata.isValid())
        return;

    const auto map = metadata.value<QVariantMap>();
    track.title = map.value("xesam:title").toString();
    const QVariant artists = map.value("xesam:artist");

    if(artists.canConvert<QStringList>())
    {
        const QStringList list = artists.toStringList();
        track.artist = list.join(", ");
    }
    else
        track.artist = artists.toString();

    track.album = map.value("xesam:album").toString();
    track.artUrl = map.value("mpris:artUrl").toString();
    track.lengthUs = map.value("mpris:length").toLongLong();
    m_track = track;
    emit trackChanged(m_track);
}

void MprisService::updatePlaybackStatus()
{
    const QVariant value = getProperty("org.mpris.MediaPlayer2.Player", "PlaybackStatus");

    if(!value.isValid())
        return;

    const QString status = value.toString();

    if(status == m_playbackStatus)
        return;

    m_playbackStatus = status;
    emit playbackStatusChanged(m_playbackStatus);
}

void MprisService::watchPlayer(const QString &service)
{
    auto dbus = QDBusConnection::sessionBus();
    dbus.connect(service, "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged", this, SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

void MprisService::unwatchPlayer(const QString &service)
{
    QDBusConnection::sessionBus().disconnect(service, "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "PropertiesChanged", this, nullptr);
}

void MprisService::onNameOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner)
{
    Q_UNUSED(oldOwner)
    Q_UNUSED(newOwner)
    if(name.startsWith("org.mpris.MediaPlayer2."))
        refreshPlayers();
}

void MprisService::onPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties)
    if(interfaceName != "org.mpris.MediaPlayer2.Player")
        return;
    if(changedProperties.contains("Metadata"))
        updateTrack();
    if(changedProperties.contains("PlaybackStatus"))
        updatePlaybackStatus();
}

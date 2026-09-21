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
    return m_players;
}

QString MprisService::currentPlayer() const
{
    return m_currentPlayer;
}

Track MprisService::currentTrack() const
{
    return m_track;
}

bool MprisService::isPlaying() const
{
    return m_playbackStatus == "Playing";
}

bool MprisService::isPaused() const
{
    return m_playbackStatus == "Paused";
}

bool MprisService::isStopped() const
{
    return m_playbackStatus == "Stopped";
}

void MprisService::play()
{
    callPlayerMethod("Play");
}

void MprisService::pause()
{
    callPlayerMethod("Pause");
}

void MprisService::playPause()
{
    callPlayerMethod("PlayPause");
}

void MprisService::stop()
{
    callPlayerMethod("Stop");
}

void MprisService::next()
{
    callPlayerMethod("Next");
}

void MprisService::previous()
{
    callPlayerMethod("Previous");
}

void MprisService::setPlayer(const QString &player)
{
    if(!m_players.contains(player))
        return;
    if(m_currentPlayer == player)
        return;
    m_currentPlayer = player;
    updateTrack();
    updatePlaybackStatus();

    emit playerChanged(m_currentPlayer);
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
    const auto bus = QDBusConnection::sessionBus();
    const QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus", "ListNames" );
    const QDBusMessage reply = bus.call(message);

    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "ListNames failed:" << reply.errorName() << reply.errorMessage();
        return;
    }

    if(reply.arguments().isEmpty())
    {
        qWarning() << "ListNames returned no arguments";
        return;
    }

    const QStringList names = reply.arguments().first().toStringList();
    QStringList players;

    for(const QString& name : names)
    {
        if(name.startsWith("org.mpris.MediaPlayer2."))
            players.append(name);
    }

    players.sort();
    if(players == m_players)
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

    QDBusInterface properties(m_currentPlayer, "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", QDBusConnection::sessionBus() );

    if(!properties.isValid())
        return {};

    const QDBusMessage reply = properties.call("Get", interfaceName, propertyName);

    if(reply.type() == QDBusMessage::ErrorMessage || reply.arguments().isEmpty())
        return {};

    const QDBusVariant outer = qvariant_cast<QDBusVariant>(reply.arguments().constFirst());

    const QVariant value = outer.variant();

    if(value.userType() == qMetaTypeId<QDBusArgument>())
        return dbusArgumentToMap(qvariant_cast<QDBusArgument>(value));

    return value;
}

void MprisService::updateTrack()
{
    Track track;
    const QVariant metadata = getProperty("org.mpris.MediaPlayer2.Player", "Metadata");

    if(!metadata.isValid())
        return;

    const auto map = metadata.value<QVariantMap>();
    track.title = map.value("xesam:title").isNull() ? map.value("xesam:url").toString() : map.value("xesam:title").toString();
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

QVariantMap MprisService::dbusArgumentToMap(const QDBusArgument &argument) const
{
    QVariantMap map;
    argument.beginMap();

    while(!argument.atEnd())
    {
        QString key;
        QDBusVariant value;
        argument.beginMapEntry();
        argument >> key >> value;
        argument.endMapEntry();
        map.insert(key, value.variant());
    }
    argument.endMap();
    return map;
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

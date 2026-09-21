#ifndef MPRISSERVICE_H
#define MPRISSERVICE_H

#include <QObject>
#include <qdbusargument.h>

struct Track
{
    QString title;
    QString artist;
    QString album;
    QString artUrl;
    qint64 lengthUs {0};
};

class MprisService : public QObject
{
    Q_OBJECT
public:
    static MprisService* instance();
    static MprisService* create(QObject* parent = nullptr);
    static void destroy();
    MprisService(const MprisService&) = delete;
    MprisService& operator=(const MprisService&) = delete;
    QStringList players() const;
    QString currentPlayer() const;
    Track currentTrack() const;
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;

public slots:
    void play();
    void pause();
    void playPause();
    void stop();
    void next();
    void previous();
    void setPlayer(const QString& player);

private:
    explicit MprisService(QObject *parent = nullptr);
    void refreshPlayers();
    void selectPlayer();
    bool callPlayerMethod(const QString& method);
    QVariant getProperty(const QString& interfaceName, const QString& propertyName) const;
    void updateTrack();
    void updatePlaybackStatus();
    void watchPlayer(const QString& service);
    void unwatchPlayer(const QString& service);
    QVariantMap dbusArgumentToMap(const QDBusArgument& argument) const;

private:
    inline static MprisService* m_instance {nullptr};
    QStringList m_players;
    QString m_currentPlayer;
    Track m_track;
    QString m_playbackStatus;

private slots:
    void onNameOwnerChanged(const QString& name, const QString& oldOwner, const QString& newOwner);
    void onPropertiesChanged(const QString& interfaceName, const QVariantMap& changedProperties, const QStringList& invalidatedProperties);

signals:
    void playersChanged();
    void playerChanged(const QString& player);
    void trackChanged(const Track& track);
    void playbackStatusChanged(const QString& status);
};

#endif // MPRISSERVICE_H

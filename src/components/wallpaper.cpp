#include "wallpaper.h"
#include <LayerShellQt/window.h>
#include <QAudioOutput>
#include <QMimeDatabase>
#include <QSettings>
#include <QMap>

Wallpaper::Wallpaper(QWidget *parent) : QWidget{parent}
{
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerBackground);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight | LayerShellQt::Window::AnchorLeft | LayerShellQt::Window::AnchorBottom));

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
}

void Wallpaper::initWallpaper()
{
    const static QMap<QString, Qt::AspectRatioMode> aspectRatioModeMap{{"ignoreaspectratio", Qt::IgnoreAspectRatio}};
    const static QMap<QString, Qt::TransformationMode> transformationModeMap{{"fast", Qt::FastTransformation}, {"smooth", Qt::SmoothTransformation}};
    QSettings settings;
    aspectratioMode = aspectRatioModeMap.value(settings.value("Wallpaper/AspectRatioMode", Qt::KeepAspectRatioByExpanding).toString().trimmed().toLower());
    transformationMode = transformationModeMap.value(settings.value("Wallpaper/TransformationMode", Qt::SmoothTransformation).toString().trimmed().toLower());
    const QString wallpaper = settings.value("Wallpaper/wallpaper", "").toString();
    if(wallpaper.isEmpty())
        return;
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(wallpaper);

    if(mimeType.inherits("image/plain") || mimeType.name().startsWith("image/"))
    {
        imageLabel = new QLabel(this);
        mainLayout->addWidget(imageLabel);
        QPixmap pixmap(wallpaper);
        imageLabel->setPixmap(pixmap.scaled(size(), aspectratioMode, transformationMode));
        currentPixmap = pixmap;
    }
    else if(mimeType.inherits("video/plain") || mimeType.name().startsWith("video/"))
    {
        videoWidget = new QVideoWidget(this);
        videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        videoWidget->setMinimumSize(100, 100);
        videoWidget->setAspectRatioMode(aspectratioMode);
        mediaPlayer = new QMediaPlayer(this);
        QAudioOutput* audioOutput = new QAudioOutput(this);

        mediaPlayer->setAudioOutput(audioOutput);
        audioOutput->setMuted(true);

        mediaPlayer->setVideoOutput(videoWidget);
        mediaPlayer->setSource(QUrl::fromLocalFile(wallpaper));
        mediaPlayer->setLoops(QMediaPlayer::Infinite);

        mainLayout->addWidget(videoWidget);
        mediaPlayer->play();
    }
}

void Wallpaper::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(imageLabel && !currentPixmap.isNull())
        imageLabel->setPixmap(currentPixmap.scaled(size(), aspectratioMode, transformationMode));
}

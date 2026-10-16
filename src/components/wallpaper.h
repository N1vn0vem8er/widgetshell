#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QMediaPlayer>
#include <QVideoWidget>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qwidget.h>

class Wallpaper : public QWidget
{
    Q_OBJECT
public:
    explicit Wallpaper(QWidget *parent = nullptr);
    void initWallpaper();

private:
    QHBoxLayout* mainLayout;
    QLabel* imageLabel;
    QVideoWidget* videoWidget;
    QMediaPlayer* mediaPlayer;
    QPixmap currentPixmap;
    Qt::AspectRatioMode aspectratioMode;
    Qt::TransformationMode transformationMode;

signals:

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // WALLPAPER_H

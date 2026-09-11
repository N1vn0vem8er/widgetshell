#ifndef NETWORKMENU_H
#define NETWORKMENU_H

#include <LayerShellQt/window.h>
#include <QWidget>

class NetworkMenu : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkMenu(QWidget *parent = nullptr);
    void showForWidget(QWidget* widget);

private:
    LayerShellQt::Window* layerWindow;

signals:
};

#endif // NETWORKMENU_H

#ifndef HYPRLANDSERVICE_H
#define HYPRLANDSERVICE_H

#include <QObject>

class HyprlandService : public QObject
{
    Q_OBJECT
public:
    explicit HyprlandService(QObject *parent = nullptr);

signals:
};

#endif // HYPRLANDSERVICE_H

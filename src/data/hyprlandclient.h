#ifndef HYPRLANDCLIENT_H
#define HYPRLANDCLIENT_H

#include "data/hyprlandworkspace.h"
#include <qobject.h>
#include <qpoint.h>
#include <qsize.h>

struct HyprlandClient {
    QString address;
    bool mapped;
    bool hidden;
    bool visible;
    bool acceptsInput;
    QPoint at;
    QSize size;
    HyprlandWorkspaceR workspace;
    bool floating;
    int monitor;
    QString appClass;
    QString title;
    QString initialClass;
    QString initialTitle;
    int pid;
    bool xwayland;
    bool pinned;
    bool pinFullscreened;
    int fullscreen;
    int fullscreenClient;
    QString fullscreenHandler;
    bool allowedOverFullscreen;
    bool tearingHint;
    long stableId;
};

#endif // HYPRLANDCLIENT_H

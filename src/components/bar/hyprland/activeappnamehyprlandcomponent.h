#ifndef ACTIVEAPPNAMEHYPRLANDCOMPONENT_H
#define ACTIVEAPPNAMEHYPRLANDCOMPONENT_H

#include "components/bar/activeappnamecomponent.h"
#include <qtmetamacros.h>
#include <qwidget.h>

class ActiveAppNameHyprlandComponent : public ActiveAppNameComponent
{
    Q_OBJECT
public:
    ActiveAppNameHyprlandComponent(QWidget* parent = nullptr);
};

#endif // ACTIVEAPPNAMEHYPRLANDCOMPONENT_H

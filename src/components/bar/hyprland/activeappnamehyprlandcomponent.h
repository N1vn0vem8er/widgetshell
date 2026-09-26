#ifndef ACTIVEAPPNAMEHYPRLANDCOMPONENT_H
#define ACTIVEAPPNAMEHYPRLANDCOMPONENT_H

#include "components/bar/activeappnamecomponent.h"
#include "services/hyprlandservice.h"
#include <qlabel.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class ActiveAppNameHyprlandComponent : public ActiveAppNameComponent
{
    Q_OBJECT
public:
    explicit ActiveAppNameHyprlandComponent(uint maxLength, QWidget* parent = nullptr);

private:
    QLabel* label;
    HyprlandService* service;
    uint maxLength {100};

private slots:
    void activeChanged(const QString&, const QString& windowTitle);
};

#endif // ACTIVEAPPNAMEHYPRLANDCOMPONENT_H

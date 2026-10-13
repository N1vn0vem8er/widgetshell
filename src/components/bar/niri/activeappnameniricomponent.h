#ifndef ACTIVEAPPNAMENIRICOMPONENT_H
#define ACTIVEAPPNAMENIRICOMPONENT_H

#include "components/bar/activeappnamecomponent.h"
#include "services/niriservice.h"
#include <qlabel.h>
class ActiveAppNameNiriComponent : public ActiveAppNameComponent
{
    Q_OBJECT
public:
    explicit ActiveAppNameNiriComponent(uint maxLength, QWidget* parent = nullptr);

private:
    QLabel* label;
    NiriService* service;
    uint maxLength {100};

private slots:
    void activeChanged(const NiriClient& client);
};

#endif // ACTIVEAPPNAMENIRICOMPONENT_H

#ifndef ACTIVEAPPNAMESWAYCOMPONENT_H
#define ACTIVEAPPNAMESWAYCOMPONENT_H

#include "components/bar/activeappnamecomponent.h"
#include "services/swayservice.h"
#include <qlabel.h>

class ActiveAppNameSwayComponent : public ActiveAppNameComponent
{
    Q_OBJECT
public:
    explicit ActiveAppNameSwayComponent(uint maxLength, QWidget* parent = nullptr);

private:
    QLabel* label;
    SwayService* service;
    uint maxLength {100};

private slots:
    void activeChanged(const SwayClient& client);
};

#endif // ACTIVEAPPNAMESWAYCOMPONENT_H

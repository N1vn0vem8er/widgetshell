#ifndef CONTROLMENUBUTTON_H
#define CONTROLMENUBUTTON_H

#include <QPushButton>
#include "components/controlmenu.h"

class ControlMenuButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ControlMenuButton(QWidget* parent = nullptr);
    ~ControlMenuButton();

private:
    ControlMenu* controlMenu {nullptr};
};

#endif // CONTROLMENUBUTTON_H

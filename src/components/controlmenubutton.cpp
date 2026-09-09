#include "controlmenubutton.h"

ControlMenuButton::ControlMenuButton(QWidget *parent) : QPushButton(parent)
{
    setIcon(QIcon::fromTheme("application-menu"));
    controlMenu = new ControlMenu();
    connect(this, &QPushButton::clicked, this, [this]{if(controlMenu->isVisible()) controlMenu->hide(); else controlMenu->show();});
}

ControlMenuButton::~ControlMenuButton()
{
    if(controlMenu)
        controlMenu->deleteLater();
}

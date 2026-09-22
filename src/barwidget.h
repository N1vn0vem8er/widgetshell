#ifndef BARWIDGET_H
#define BARWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QWidget>

#include "menus/controlmenu.h"

class BarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BarWidget(QWidget *parent = nullptr);
    ~BarWidget();

private:
    QGridLayout* mainLayout;
    ControlMenu* controlMenu {nullptr};

private:
    void createBar();

signals:
};

#endif // BARWIDGET_H

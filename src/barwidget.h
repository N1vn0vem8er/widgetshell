#ifndef BARWIDGET_H
#define BARWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QWidget>

#include "components/workspaceswitchercomponent.h"

class BarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BarWidget(QWidget *parent = nullptr);

private:
    QHBoxLayout* mainLayout;
    QLabel* dateTimeLabel;
    QTimer* dateTimeTimer;
    QSpacerItem* spacerItem;
    QSpacerItem* spacerItem2;
    WorkspaceSwitcherComponent* workspaceSwitcher;

signals:
};

#endif // BARWIDGET_H

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
    QGridLayout* mainLayout;
    QLabel* dateTimeLabel;
    QTimer* dateTimeTimer;
    WorkspaceSwitcherComponent* workspaceSwitcher;

signals:
};

#endif // BARWIDGET_H

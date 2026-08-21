#ifndef BARWIDGET_H
#define BARWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QWidget>

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

signals:
};

#endif // BARWIDGET_H

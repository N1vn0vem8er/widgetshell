#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include <QWidget>

class AppLauncher : public QWidget
{
    Q_OBJECT
public:
    explicit AppLauncher(QWidget *parent = nullptr);

private slots:
    void onActivate();

signals:
};

#endif // APPLAUNCHER_H

#include "notificationpopupcomponent.h"
#include <LayerShellQt/window.h>
#include <QToolButton>
#include <qaction.h>
#include <qboxlayout.h>
#include <qpushbutton.h>

NotificationPopupComponent::NotificationPopupComponent(const Notification &notification, QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedWidth(360);
    setMinimumHeight(82);
    setMaximumHeight(220);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerTop);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* headerLayout = new QHBoxLayout();

    QLabel* icon = new QLabel(this);
    icon->setFixedSize(40, 40);
    icon->setScaledContents(true);

    if(!notification.appIcon.isEmpty())
    {
        if(QUrl(notification.appIcon).isValid())
            icon->setPixmap(QIcon(notification.appIcon).pixmap(40, 40));
        else
            icon->setPixmap(QIcon::fromTheme(notification.appIcon).pixmap(40, 40));
    }

    QVBoxLayout* textLayout = new QVBoxLayout();

    QLabel* appName = new QLabel(notification.appName, this);
    appName->setObjectName("appName");

    QLabel* summary = new QLabel(notification.summary, this);
    summary->setObjectName("summary");
    summary->setWordWrap(true);

    textLayout->addWidget(appName);
    textLayout->addWidget(summary);
    headerLayout->addWidget(icon);
    headerLayout->addLayout(textLayout, 1);

    QToolButton* closeButton = new QToolButton(this);
    closeButton->setText(QStringLiteral("x"));
    closeButton->setObjectName("closeButton");
    closeButton->setFixedSize(24, 24);
    closeButton->setCursor(Qt::PointingHandCursor);

    connect(closeButton, &QToolButton::clicked, this, [this]{hide(); emit closed();});

    headerLayout->addWidget(closeButton, 0, Qt::AlignTop);
    mainLayout->addLayout(headerLayout);

    if(!notification.body.isEmpty())
    {
        body = new QLabel(notification.body);
        body->setObjectName("body");
        body->setWordWrap(true);
        body->setTextInteractionFlags(Qt::TextSelectableByMouse);

        mainLayout->addWidget(body);
    }

    if(!notification.actions.isEmpty())
    {
        QHBoxLayout* actionsLayout = new QHBoxLayout();
        for(const QString &action : notification.actions)
        {
            QPushButton* button = new QPushButton(action, this);
            button->setObjectName("actionButton");
            button->setCursor(Qt::PointingHandCursor);
            actionsLayout->addWidget(button);
        }

        actionsLayout->addStretch();
        mainLayout->addLayout(actionsLayout);
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]{hide(); emit closed();});
    timer->setInterval(notification.expireTimeout <= 0 ? 5000 : notification.expireTimeout);
    timer->setSingleShot(true);
    timer->start();
}
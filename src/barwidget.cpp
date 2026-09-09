#include "barwidget.h"
#include "widgetfactory.h"
#include <LayerShellQt/Window>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

BarWidget::BarWidget(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(100, 50);
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerTop);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft | LayerShellQt::Window::AnchorRight));
    layerWindow->setExclusiveZone(height());

    mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // openedApps = new OpenedAppsComponent(this);

    // mainLayout->addWidget(openedApps, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    // workspaceSwitcher = new WorkspaceSwitcherComponent(this);
    // mainLayout->addWidget(workspaceSwitcher, 0, 1, Qt::AlignCenter);

    // controlMenuButton = new QPushButton(QIcon::fromTheme("application-menu"), "", this);
    // dateTimeLabel = new QLabel(this);
    // QWidget* rightWidget = new QWidget(this);
    // QHBoxLayout* rightLayout = new QHBoxLayout(rightWidget);
    // rightLayout->addWidget(dateTimeLabel);
    // rightLayout->addWidget(controlMenuButton);

    // controlMenu = new ControlMenu();
    // connect(controlMenuButton, &QPushButton::clicked, this, [this]{if(controlMenu->isVisible()) controlMenu->hide(); else controlMenu->show();});

    // mainLayout->addWidget(rightWidget, 0, 2, Qt::AlignRight | Qt::AlignVCenter);
    // dateTimeTimer = new QTimer(this);
    // connect(dateTimeTimer, &QTimer::timeout, this, [this]{dateTimeLabel->setText(QDateTime::currentDateTime().toString("hh::mm:ss | dd:MM:yyyy"));});
    // dateTimeTimer->setInterval(1000);
    // dateTimeTimer->start();

    createBar();
}

BarWidget::~BarWidget()
{
    if(controlMenu)
        controlMenu->deleteLater();
}

void BarWidget::createBar()
{
    QFile configFile(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/layout.json");
    if(!configFile.open(QIODevice::ReadOnly))
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll());
    configFile.close();

    if(!doc.isObject())
        return;

    const QJsonObject root = doc.object();
    WidgetFactory factory;
    if(root.contains("left") && root.value("left").isArray())
    {
        const QJsonArray layoutArray = root.value("left").toArray();
        QHBoxLayout* layout = new QHBoxLayout();
        for(const auto& val : layoutArray)
        {
            const QJsonObject element = val.toObject();
            if(element.contains("component"))
                layout->addWidget(factory.create(element.value("component").toString(), element, this));
        }
        mainLayout->addLayout(layout, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    }
    if(root.contains("center") && root.value("center").isArray())
    {
        const QJsonArray layoutArray = root.value("center").toArray();
        QHBoxLayout* layout = new QHBoxLayout();
        for(const auto& val : layoutArray)
        {
            const QJsonObject element = val.toObject();
            if(element.contains("component"))
                layout->addWidget(factory.create(element.value("component").toString(), element, this));
        }
        mainLayout->addLayout(layout, 0, 1, Qt::AlignCenter);
    }
    if(root.contains("right") && root.value("right").isArray())
    {
        const QJsonArray layoutArray = root.value("right").toArray();
        QHBoxLayout* layout = new QHBoxLayout();
        for(const auto& val : layoutArray)
        {
            const QJsonObject element = val.toObject();
            if(element.contains("component"))
                layout->addWidget(factory.create(element.value("component").toString(), element, this));
        }
        mainLayout->addLayout(layout, 0, 2, Qt::AlignRight | Qt::AlignVCenter);
    }
}

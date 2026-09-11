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
#include <qsettings.h>

BarWidget::BarWidget(QWidget *parent)
    : QWidget{parent}
{
    QSettings settings;
    settings.beginGroup("Bar");
    setFixedSize(settings.value("barWidth", 100).toInt(), settings.value("barHeight", 50).toInt());
    settings.endGroup();
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerTop);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft | LayerShellQt::Window::AnchorRight));
    layerWindow->setExclusiveZone(height());

    mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    createBar();
}

BarWidget::~BarWidget()
{
    if(controlMenu)
        controlMenu->deleteLater();
}

void BarWidget::createBar()
{
    QFile configFile(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/" + QCoreApplication::organizationName() + "/layout.json");
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

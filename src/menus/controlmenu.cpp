#include "controlmenu.h"

#include <LayerShellQt/Window>

#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <qdir.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <widgetfactory.h>

ControlMenu::ControlMenu(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StyledBackground, true);
    QSettings settings;
    setFixedSize(settings.value("ControlMenu/width", 400).toInt(), settings.value("ControlMenu/height", 600).toInt());
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight));
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    contentWidget = new QWidget();
    mainLayout = new QVBoxLayout(contentWidget);

    scrollArea->setWidget(contentWidget);
    layout->addWidget(scrollArea);

    createMenu();
}

void ControlMenu::createMenu()
{
    QFileInfo fi(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/" + QCoreApplication::organizationName() + "/controlmenu.json");
    QFile configFile(fi.exists() ? fi.absoluteFilePath() : ":/config/controlmenu.json");
    if(!configFile.open(QIODevice::ReadOnly))
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll());
    configFile.close();

    if(!doc.isArray())
        return;

    const QJsonArray root = doc.array();
    WidgetFactory factory;

    for(const auto& val : root)
    {
        const QJsonObject element = val.toObject();
        if(element.contains("component"))
            mainLayout->addWidget(factory.create(element.value("component").toString(), element, contentWidget));
    }
}

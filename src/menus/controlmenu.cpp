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
    setFixedSize(settings.value("ControlMenu/width", 300).toInt(), settings.value("ControlMenu/height", 600).toInt());
    winId();
    LayerShellQt::Window* layerWindow = LayerShellQt::Window::get(windowHandle());
    layerWindow->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
    layerWindow->setAnchors(LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight));
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);

    mainLayout = new QVBoxLayout(this);
    createMenu();
}

void ControlMenu::createMenu()
{
    QFile configFile(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/" + QCoreApplication::organizationName() + "/controlmenu.json");
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
            mainLayout->addWidget(factory.create(element.value("component").toString(), element, this));
    }
}

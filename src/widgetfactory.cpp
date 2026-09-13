#include "widgetfactory.h"
#include "components/controlmenubutton.h"
#include "components/cpuusagecomponent.h"
#include "components/datetimecomponent.h"
#include "components/memoryusagecomponent.h"
#include "components/networkconnectioncomponent.h"
#include "components/openedappscomponent.h"
#include "components/workspaceswitchercomponent.h"

#include <QJsonObject>

WidgetFactory::WidgetFactory(QObject *parent)
    : QObject{parent}
{
    registerWidget("workspaceswitcher", [](const QJsonObject& obj, QWidget* parent){
        WorkspaceSwitcherComponent* widget = new WorkspaceSwitcherComponent(parent);
        return widget;
    });
    registerWidget("datetime", [](const QJsonObject& obj, QWidget* parent){
        QString format = "hh::mm:ss | dd:MM:yyyy";
        if(obj.contains("format"))
            format = obj.value("format").toString();
        DateTimeComponent* widget = new DateTimeComponent(format, parent);
        return widget;
    });
    registerWidget("openedapps", [](const QJsonObject& obj, QWidget* parent){
        OpenedAppsComponent* widget = new OpenedAppsComponent(parent);
        return widget;
    });
    registerWidget("controlmenu", [](const QJsonObject& obj, QWidget* parent){
        ControlMenuButton* widget = new ControlMenuButton(parent);
        return widget;
    });
    registerWidget("cpuusage", [](const QJsonObject& obj, QWidget* parent){
        CpuUsageComponent* widget = new CpuUsageComponent(obj.value("format").toString("%1%"), parent);
        return widget;
    });
    registerWidget("memoryusage", [](const QJsonObject& obj, QWidget* parent){
        MemoryUsageComponent* widget;
        if(obj.contains("unit"))
        {
            const QString unit = obj.value("unit").toString().trimmed().toLower();
            if(unit == "tb")
                widget = new MemoryUsageComponent(obj.value("format").toString("%1 %3 / %2 %3"), MemoryUsageComponent::Unit::TB, parent);
            else if(unit == "mb")
                widget = new MemoryUsageComponent(obj.value("format").toString("%1 %3 / %2 %3"), MemoryUsageComponent::Unit::MB, parent);
            else if(unit == "gb")
                widget = new MemoryUsageComponent(obj.value("format").toString("%1 %3 / %2 %3"), MemoryUsageComponent::Unit::GB, parent);
            else
                widget = new MemoryUsageComponent(obj.value("format").toString("%1 %3 / %2 %3"), MemoryUsageComponent::Unit::KB, parent);
        }
        else
            widget = new MemoryUsageComponent(obj.value("format").toString("%1 %3 / %2 %3"), MemoryUsageComponent::Unit::KB, parent);
        return widget;
    });
    registerWidget("networkconnection", [](const QJsonObject& obj, QWidget* parent){
        NetworkConnectionComponent* widget;
        if(obj.contains("unit"))
        {
            const QString unit = obj.value("unit").toString().trimmed().toLower();
            if(unit == "b")
                widget = new NetworkConnectionComponent(NetworkConnectionComponent::Unit::B, parent);
            else if(unit == "kb")
                widget = new NetworkConnectionComponent(NetworkConnectionComponent::Unit::KB, parent);
            else if(unit == "gb")
                widget = new NetworkConnectionComponent(NetworkConnectionComponent::Unit::GB, parent);
            else if(unit == "tb")
                widget = new NetworkConnectionComponent(NetworkConnectionComponent::Unit::TB, parent);
            else
                widget = new NetworkConnectionComponent(NetworkConnectionComponent::Unit::MB, parent);
        }
        else
            widget = new NetworkConnectionComponent(NetworkConnectionComponent::Unit::MB, parent);
        return widget;
    });
}

void WidgetFactory::registerWidget(const QString &name, Creator creator)
{
    m_creators[name] = std::move(creator);
}

QWidget *WidgetFactory::create(const QString &name, const QJsonObject& obj, QWidget *parent) const
{
    auto it = m_creators.find(name);

    if(it == m_creators.end())
        return nullptr;

    return it.value()(obj, parent);
}

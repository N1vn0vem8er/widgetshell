#include "widgetfactory.h"
#include "components/bar/audiovolumecontrolbarcomponent.h"
#include "components/bar/controlmenubutton.h"
#include "components/bar/cpuusagecomponent.h"
#include "components/bar/datetimecomponent.h"
#include "components/bar/mediaplayerbarcomponent.h"
#include "components/bar/memoryusagecomponent.h"
#include "components/bar/networkconnectioncomponent.h"
#include "components/bar/hyprland/openedappshyprlandcomponent.h"
#include "components/bar/uptimebarcomponent.h"
#include "components/bar/hyprland/workspaceswitcherhyprlandcomponent.h"

#include <QJsonObject>

WidgetFactory::WidgetFactory(QObject *parent)
    : QObject{parent}
{
    registerWidget("workspaceswitcher", [](const QJsonObject& obj, QWidget* parent){
        WorkspaceSwitcherHyprlandComponent* widget = new WorkspaceSwitcherHyprlandComponent(parent);
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
        OpenedAppsHyprlandComponent* widget = new OpenedAppsHyprlandComponent(parent);
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
    registerWidget("audiocontrol", [](const QJsonObject& obj, QWidget* parent){
        AudioVolumeControlBarComponent* widget = new AudioVolumeControlBarComponent(parent);
        return widget;
    });
    registerWidget("uptime", [](const QJsonObject& obj, QWidget* parent){
        UptimeBarComponent* widget = new UptimeBarComponent(parent);
        return widget;
    });
    registerWidget("mediaplayer", [](const QJsonObject& obj, QWidget* parent){
        MediaPlayerBarComponent* widget = new MediaPlayerBarComponent(parent);
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

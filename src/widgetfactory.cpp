#include "widgetfactory.h"
#include "components/bar/applauncherbutton.h"
#include "components/bar/audiovolumecontrolbarcomponent.h"
#include "components/bar/controlmenubutton.h"
#include "components/bar/cpuusagecomponent.h"
#include "components/bar/datetimecomponent.h"
#include "components/bar/diskusagebarcomponent.h"
#include "components/bar/hyprland/activeappnamehyprlandcomponent.h"
#include "components/bar/mediaplayerbarcomponent.h"
#include "components/bar/memoryusagecomponent.h"
#include "components/bar/networkconnectioncomponent.h"
#include "components/bar/hyprland/openedappshyprlandcomponent.h"
#include "components/bar/niri/activeappnameniricomponent.h"
#include "components/bar/niri/openedappsniricomponent.h"
#include "components/bar/niri/workspaceswitcherniricomponent.h"
#include "components/bar/storageusagebarcomponent.h"
#include "components/bar/sway/activeappnameswaycomponent.h"
#include "components/bar/sway/openedappsswaycomponent.h"
#include "components/bar/sway/workspaceswitcherswaycomponent.h"
#include "components/bar/uptimebarcomponent.h"
#include "components/bar/hyprland/workspaceswitcherhyprlandcomponent.h"
#include "components/menus/cpuusagechartomponent.h"
#include "components/menus/memoryusagechartcomponent.h"
#include "components/menus/systemactionsmenucomponent.h"

#include <QJsonObject>

WidgetFactory::WidgetFactory(QObject *parent)
    : QObject{parent}
{
    registerWidget("workspaceswitcher", [](const QJsonObject& obj, QWidget* parent){
        QWidget* widget;
        if(getenv("HYPRLAND_INSTANCE_SIGNATURE") != NULL)
        {
            widget = new WorkspaceSwitcherHyprlandComponent(parent);
        }
        else if(getenv("SWAYSOCK") != NULL)
        {
            widget = new WorkspaceSwitcherSwayComponent(parent);
        }
        else if(getenv("NIRI_SOCKET") != NULL)
        {
            widget = new WorkspaceSwitcherNiriComponent(parent);
        }
        else
            widget = new QWidget(parent);
        return widget;
    });
    registerWidget("activeappname", [](const QJsonObject& obj, QWidget* parent){
        QWidget* widget;
        uint maxLength = obj.value("maxLength").toInt(100);
        if(getenv("HYPRLAND_INSTANCE_SIGNATURE") != NULL)
        {
            widget = new ActiveAppNameHyprlandComponent(maxLength, parent);
        }
        else if(getenv("SWAYSOCK") != NULL)
        {
            widget = new ActiveAppNameSwayComponent(maxLength, parent);
        }
        else if(getenv("NIRI_SOCKET") != NULL)
        {
            widget = new ActiveAppNameNiriComponent(maxLength, parent);
        }
        else
            widget = new QWidget(parent);
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
        QWidget* widget;
        if(getenv("HYPRLAND_INSTANCE_SIGNATURE") != NULL)
        {
            widget = new OpenedAppsHyprlandComponent(parent);
        }
        else if(getenv("SWAYSOCK") != NULL)
        {
            widget = new OpenedAppsSwayComponent(parent);
        }
        else if(getenv("NIRI_SOCKET") != NULL)
        {
            widget = new OpenedAppsNiriComponent(parent);
        }
        else
            widget = new QWidget(parent);
        return widget;
    });
    registerWidget("controlmenu", [](const QJsonObject& obj, QWidget* parent){
        ControlMenuButton* widget = new ControlMenuButton(parent);
        return widget;
    });
    registerWidget("applauncherbutton", [](const QJsonObject& obj, QWidget* parent){
        AppLauncherButton* widget = new AppLauncherButton(obj.value("icon").toString("application-x-executable"), obj.value("text").toString(), parent);
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
    registerWidget("storageusage", [](const QJsonObject& obj, QWidget* parent){
        StorageUsageBarComponent* widget;
        if(obj.contains("unit"))
        {
            const QString unit = obj.value("unit").toString().trimmed().toLower();
            if(unit == "tb")
                widget = new StorageUsageBarComponent(obj.value("format").toString("%1%3 / %2%3"), StorageUsageBarComponent::Unit::TB, parent);
            else if(unit == "mb")
                widget = new StorageUsageBarComponent(obj.value("format").toString("%1%3 / %2%3"), StorageUsageBarComponent::Unit::MB, parent);
            else if(unit == "gb")
                widget = new StorageUsageBarComponent(obj.value("format").toString("%1%3 / %2%3"), StorageUsageBarComponent::Unit::GB, parent);
            else
                widget = new StorageUsageBarComponent(obj.value("format").toString("%1%3 / %2%3"), StorageUsageBarComponent::Unit::KB, parent);
        }
        else
            widget = new StorageUsageBarComponent(obj.value("format").toString("%1%3 / %2%3"), StorageUsageBarComponent::Unit::KB, parent);
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
        UptimeBarComponent* widget = new UptimeBarComponent(obj.value("format").toString("%1d %2h %3m %4s"), parent);
        return widget;
    });
    registerWidget("mediaplayer", [](const QJsonObject& obj, QWidget* parent){
        MediaPlayerBarComponent* widget = new MediaPlayerBarComponent(parent);
        return widget;
    });
    registerWidget("systemactions", [](const QJsonObject& obj, QWidget* parent){
        SystemActionsMenuComponent* widget = new SystemActionsMenuComponent(parent);
        return widget;
    });
    registerWidget("menuaudiocontrol", [](const QJsonObject& obj, QWidget* parent){
        AudioVolumeControlComponent* widget = new AudioVolumeControlComponent(parent);
        return widget;
    });
    registerWidget("menunotificationhistory", [](const QJsonObject& obj, QWidget* parent){
        NotificationHistoryComponent* widget = new NotificationHistoryComponent(parent);
        return widget;
    });
    registerWidget("menucalendar", [](const QJsonObject& obj, QWidget* parent){
        QCalendarWidget* widget = new QCalendarWidget(parent);
        return widget;
    });
    registerWidget("cpuusagechart", [](const QJsonObject& obj, QWidget* parent){
        const QString chart = obj.value("chart").toString("pie").trimmed().toLower();
        CpuUsageChartComponent* widget;
        if(chart == "pie")
            widget = new CpuUsageChartComponent(CpuUsageChartComponent::ChartType::Pie, parent);
        else if(chart == "bar")
            widget = new CpuUsageChartComponent(CpuUsageChartComponent::ChartType::Bar, parent);
        else if(chart == "line")
            widget = new CpuUsageChartComponent(CpuUsageChartComponent::ChartType::Line, parent);
        else
            widget = new CpuUsageChartComponent(CpuUsageChartComponent::ChartType::Pie, parent);
        return widget;
    });
    registerWidget("memoryusagechart", [](const QJsonObject& obj, QWidget* parent){
        const QString chart = obj.value("chart").toString("pie").trimmed().toLower();
        MemoryUsageChartComponent* widget;
        if(chart == "pie")
            widget = new MemoryUsageChartComponent(MemoryUsageChartComponent::ChartType::Pie, parent);
        else if(chart == "bar")
            widget = new MemoryUsageChartComponent(MemoryUsageChartComponent::ChartType::Bar, parent);
        else if(chart == "line")
            widget = new MemoryUsageChartComponent(MemoryUsageChartComponent::ChartType::Line, parent);
        else
            widget = new MemoryUsageChartComponent(MemoryUsageChartComponent::ChartType::Pie, parent);
        return widget;
    });
    registerWidget("diskusage", [](const QJsonObject& obj, QWidget* parent){
        DiskUsageBarComponent* widget;
        if(obj.contains("unit"))
        {
            const QString unit = obj.value("unit").toString().trimmed().toLower();
            if(unit == "tb")
                widget = new DiskUsageBarComponent(obj.value("disk").toString(), obj.value("format").toString("%1 ↑ %2 %4 | ↓ %3 %4"), DiskUsageBarComponent::Unit::TB, parent);
            else if(unit == "mb")
                widget = new DiskUsageBarComponent(obj.value("disk").toString(), obj.value("format").toString("%1 ↑ %2 %4 | ↓ %3 %4"), DiskUsageBarComponent::Unit::MB, parent);
            else if(unit == "gb")
                widget = new DiskUsageBarComponent(obj.value("disk").toString(), obj.value("format").toString("%1 ↑ %2 %4 | ↓ %3 %4"), DiskUsageBarComponent::Unit::GB, parent);
            else
                widget = new DiskUsageBarComponent(obj.value("disk").toString(), obj.value("format").toString("%1 ↑ %2 %4 | ↓ %3 %4"), DiskUsageBarComponent::Unit::KB, parent);
        }
        else
            widget = new DiskUsageBarComponent(obj.value("disk").toString(), obj.value("format").toString("%1 ↑ %2 %4 | ↓ %3 %4"), DiskUsageBarComponent::Unit::KB, parent);
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

cmake_minimum_required(VERSION 3.19)
project(widgetshell LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 23)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network DBus)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)
find_package(LayerShellQt REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PULSEAUDIO REQUIRED libpulse)

if(NOT DEFINED ENV{QT_QPA_PLATFORMTHEME})
    message(WARNING "QT_QPA_PLATFORMTHEME not defined" OR "$ENV{QT_QPA_PLATFORMTHEME}" STREQUAL "")
endif()

qt_standard_project_setup()

include_directories(src)
include_directories(${PULSEAUDIO_INCLUDE_DIRS})

set(CMAKE_AUTORCC ON)

qt_add_executable(widgetshell
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/barwidget.h src/barwidget.cpp
    src/components/wallpaper.h src/components/wallpaper.cpp

    src/services/hyprlandservice.h src/services/hyprlandservice.cpp
    src/services/audioservice.h src/services/audioservice.cpp
    src/services/desktopappsservice.h src/services/desktopappsservice.cpp
    src/services/notificationservice.h src/services/notificationservice.cpp
    src/services/mprisservice.h src/services/mprisservice.cpp
    src/services/systemmonitorservice.h src/services/systemmonitorservice.cpp
    src/services/networkservice.h src/services/networkservice.cpp

    src/widgetfactory.h src/widgetfactory.cpp

    src/components/bar/hyprland/openedappshyprlandcomponent.h src/components/bar/hyprland/openedappshyprlandcomponent.cpp
    src/components/bar/hyprland/workspaceswitcherhyprlandcomponent.h src/components/bar/hyprland/workspaceswitcherhyprlandcomponent.cpp

    src/components/bar/workspacebutton.h src/components/bar/workspacebutton.cpp
    src/components/bar/audiovolumecontrolbarcomponent.h src/components/bar/audiovolumecontrolbarcomponent.cpp
    src/components/bar/uptimebarcomponent.h src/components/bar/uptimebarcomponent.cpp
    src/components/bar/mediaplayerbarcomponent.h src/components/bar/mediaplayerbarcomponent.cpp
    src/components/bar/cpuusagecomponent.h src/components/bar/cpuusagecomponent.cpp
    src/components/bar/memoryusagecomponent.h src/components/bar/memoryusagecomponent.cpp
    src/components/bar/datetimecomponent.h src/components/bar/datetimecomponent.cpp
    src/components/bar/controlmenubutton.h src/components/bar/controlmenubutton.cpp
    src/components/bar/networkconnectioncomponent.h src/components/bar/networkconnectioncomponent.cpp

    src/menus/networkmenu.h src/menus/networkmenu.cpp
    src/menus/controlmenu.h src/menus/controlmenu.cpp

    src/components/menus/audiovolumecontrolcomponent.h src/components/menus/audiovolumecontrolcomponent.cpp
    src/components/menus/notificationhistorycomponent.h src/components/menus/notificationhistorycomponent.cpp

    src/components/notificationpopupcomponent.h src/components/notificationpopupcomponent.cpp
    src/controllers/notificationpopupscontroller.h src/controllers/notificationpopupscontroller.cpp

    resources/resources.qrc
    src/services/swayservice.h src/services/swayservice.cpp
    src/components/bar/sway/openedappsswaycomponent.h src/components/bar/sway/openedappsswaycomponent.cpp
    src/components/bar/sway/workspaceswitcherswaycomponent.h src/components/bar/sway/workspaceswitcherswaycomponent.cpp
    src/components/bar/openedappscomponent.h
    src/components/bar/activeappnamecomponent.h
    src/components/bar/hyprland/activeappnamehyprlandcomponent.h src/components/bar/hyprland/activeappnamehyprlandcomponent.cpp
    src/components/bar/sway/activeappnameswaycomponent.h src/components/bar/sway/activeappnameswaycomponent.cpp
    src/components/bar/workspaceswitchercomponent.h
    src/components/bar/storageusagebarcomponent.h src/components/bar/storageusagebarcomponent.cpp
    src/components/menus/systemactionsmenucomponent.h src/components/menus/systemactionsmenucomponent.cpp
    src/components/bar/diskusagebarcomponent.h src/components/bar/diskusagebarcomponent.cpp
    src/services/niriservice.h src/services/niriservice.cpp
    src/components/bar/niri/workspaceswitcherniricomponent.h src/components/bar/niri/workspaceswitcherniricomponent.cpp
    src/components/bar/niri/openedappsniricomponent.h src/components/bar/niri/openedappsniricomponent.cpp
    src/components/bar/niri/activeappnameniricomponent.h src/components/bar/niri/activeappnameniricomponent.cpp
)

target_link_libraries(widgetshell
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        Qt::DBus
        LayerShellQt::Interface
)
target_link_libraries(widgetshell
    PRIVATE
        Qt6::MultimediaWidgets
        ${PULSEAUDIO_LIBRARIES})

install(TARGETS widgetshell
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET widgetshell
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

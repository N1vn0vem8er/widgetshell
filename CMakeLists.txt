cmake_minimum_required(VERSION 3.19)
project(widgetshell LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 23)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network DBus)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)
find_package(LayerShellQt REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PULSEAUDIO REQUIRED libpulse)

include_directories(${PULSEAUDIO_INCLUDE_DIRS})

qt_standard_project_setup()

include_directories(src)

set(CMAKE_AUTORCC ON)

qt_add_executable(widgetshell
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/barwidget.h src/barwidget.cpp
    src/services/hyprlandservice.h src/services/hyprlandservice.cpp
    src/components/workspaceswitchercomponent.h src/components/workspaceswitchercomponent.cpp

    src/components/controlmenu.h src/components/controlmenu.cpp
    src/components/wallpaper.h src/components/wallpaper.cpp
    src/services/audioservice.h src/services/audioservice.cpp
    src/components/audiovolumecontrolcomponent.h src/components/audiovolumecontrolcomponent.cpp

    src/components/openedappscomponent.h src/components/openedappscomponent.cpp
    src/services/desktopappsservice.h src/services/desktopappsservice.cpp
    src/widgetfactory.h src/widgetfactory.cpp
    src/components/datetimecomponent.h src/components/datetimecomponent.cpp
    src/components/controlmenubutton.h src/components/controlmenubutton.cpp
    src/services/systemmonitorservice.h src/services/systemmonitorservice.cpp
    src/components/cpuusagecomponent.h src/components/cpuusagecomponent.cpp
    src/components/memoryusagecomponent.h src/components/memoryusagecomponent.cpp
    resources/resources.qrc
    src/components/workspacebutton.h src/components/workspacebutton.cpp
    src/services/networkservice.h src/services/networkservice.cpp
    src/components/networkconnectioncomponent.h src/components/networkconnectioncomponent.cpp
    src/menus/networkmenu.h src/menus/networkmenu.cpp
    src/components/audiovolumecontrolbarcomponent.h src/components/audiovolumecontrolbarcomponent.cpp
    src/components/uptimebarcomponent.h src/components/uptimebarcomponent.cpp
    src/services/notificationservice.h src/services/notificationservice.cpp
    src/components/notificationpopupcomponent.h src/components/notificationpopupcomponent.cpp
    src/controllers/notificationpopupscontroller.h src/controllers/notificationpopupscontroller.cpp
    src/components/notificationhistorycomponent.h src/components/notificationhistorycomponent.cpp
    src/services/mprisservice.h src/services/mprisservice.cpp
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

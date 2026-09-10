cmake_minimum_required(VERSION 3.19)
project(widgetshell LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 23)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)
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
    src/data/hyprlandworkspace.h
    src/components/controlmenu.h src/components/controlmenu.cpp
    src/components/wallpaper.h src/components/wallpaper.cpp
    src/services/audioservice.h src/services/audioservice.cpp
    src/components/audiovolumecontrolcomponent.h src/components/audiovolumecontrolcomponent.cpp
    src/data/hyprlandclient.h
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
)

target_link_libraries(widgetshell
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
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

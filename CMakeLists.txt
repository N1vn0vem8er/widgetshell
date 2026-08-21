cmake_minimum_required(VERSION 3.19)
project(widgetshell LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)
find_package(LayerShellQt REQUIRED)

qt_standard_project_setup()

include_directories(src)

qt_add_executable(widgetshell
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/barwidget.h src/barwidget.cpp
    src/services/hyprlandservice.h src/services/hyprlandservice.cpp



)

target_link_libraries(widgetshell
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        LayerShellQt::Interface
)

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

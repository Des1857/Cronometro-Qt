cmake_minimum_required(VERSION 3.21)

project(Cronometro
    VERSION 1.0.0
    DESCRIPTION "Cronometro desarrollado en C++ con Qt"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(Cronometro
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/Cronometro.cpp
    src/Cronometro.h

    resources.rc
)

qt_add_resources(Cronometro "recursos"
    PREFIX "/"
    FILES
        icon.ico
)

target_link_libraries(Cronometro PRIVATE Qt6::Widgets)

set_target_properties(Cronometro PROPERTIES
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "${PROJECT_DESCRIPTION}"
)
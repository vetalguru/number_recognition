cmake_minimum_required(VERSION 3.25)

project(${GUI_RECOGNITION_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6
    REQUIRED
    COMPONENTS
    Widgets
    Gui
    Core
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    drawwidget.cpp
)

set(HEADERS
    mainwindow.h
    drawwidget.h
)

add_executable(${GUI_RECOGNITION_NAME} ${SOURCES} ${HEADER})

target_link_libraries(
    ${GUI_RECOGNITION_NAME}
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
)

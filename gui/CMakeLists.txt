cmake_minimum_required(VERSION 3.25)

project(${GUI_RECOGNITION_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6
    REQUIRED
    COMPONENTS
    Widgets
    Concurrent
    Gui
    Core
)

find_package(Boost 1.74.0
    REQUIRED
    COMPONENTS
    json)

set(SOURCES
    ${GUI_RECOGNITION_DIR}/src/main.cpp
    ${GUI_RECOGNITION_DIR}/src/mainwindow.cpp
    ${GUI_RECOGNITION_DIR}/src/drawwidget.cpp
    ${GUI_RECOGNITION_DIR}/src/mnistlearningform.cpp
)

set(HEADERS
    ${GUI_RECOGNITION_DIR}/include/mainwindow.h
    ${GUI_RECOGNITION_DIR}/include/drawwidget.h
    ${GUI_RECOGNITION_DIR}/include/mnistlearningform.h
)

add_executable(${GUI_RECOGNITION_NAME} ${SOURCES} ${HEADERS})

target_include_directories(
    ${GUI_RECOGNITION_NAME}
    PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${GUI_RECOGNITION_DIR}
    ${LIB_RECOGNITION_DIR}
)

target_link_libraries(
    ${GUI_RECOGNITION_NAME}
    PRIVATE
    Qt6::Widgets
    Qt6::Concurrent
    Qt6::Gui
    Qt6::Core
    ${LIB_RECOGNITION_NAME}
    ${Boost_LIBRARIES}
)

if(NOT EXISTS ${MNIST_OUTPUT_DIR})
    message(STATUS "Extracting MNIST dataset...")

    file(ARCHIVE_EXTRACT
      INPUT ${MNIST_ZIP}
      DESTINATION ${MNIST_OUTPUT_DIR}
    )

    message(STATUS "MNIST dataset extracted to ${MNIST_OUTPUT_DIR}")
else()
    message(STATUS "MNIST dataset already extracted to ${MNIST_OUTPUT_DIR}")
endif()

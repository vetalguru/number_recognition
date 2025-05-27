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
    ${GUI_RECOGNITION_DIR}/include/mainwindow.hpp
    ${GUI_RECOGNITION_DIR}/include/drawwidget.hpp
    ${GUI_RECOGNITION_DIR}/include/mnistlearningform.hpp
)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULTS_VARIABLE GIT_CMD_STATUS
)

string(STRIP "${GIT_COMMIT_HASH}" GIT_COMMIT_HASH)
if(NOT GIT_COMMIT_HASH STREQUAL "")
    set(VERSION_BUILD ${GIT_COMMIT_HASH})
else()
    set(VERSION_BUILD "unknown")
endif()

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)

message(STATUS "Git command exit status: ${GIT_CMD_STATUS}")
message(STATUS "Git commit hash: '${GIT_COMMIT_HASH}'")
message(STATUS "VERSION BUILD: '${VERSION_BUILD}'")

configure_file(
    ${CMAKE_SOURCE_DIR}/version.h.in
    ${CMAKE_BINARY_DIR}/gui/guiversion.h
    @ONLY
)

add_executable(${GUI_RECOGNITION_NAME} ${SOURCES} ${HEADERS})

target_include_directories(
    ${GUI_RECOGNITION_NAME}
    PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${GUI_RECOGNITION_DIR}
    ${LIB_RECOGNITION_DIR}
    ${CMAKE_BINARY_DIR}/gui
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

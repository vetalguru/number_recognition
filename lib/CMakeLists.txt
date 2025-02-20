cmake_minimum_required(VERSION 3.28)

project(${RECOGNITION_LIBRARY_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES_LIST src/helloworld.cpp
                 src/perceptron.cpp
                 src/neuron.cpp)

add_library(
    ${RECOGNITION_LIBRARY_NAME}
    SHARED
    ${SOURCES_LIST}
)

target_include_directories(
    ${RECOGNITION_LIBRARY_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

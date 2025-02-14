cmake_minimum_required(VERSION 3.28)

project(${RECOGNITION_LIBRARY_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS_LIST include/helloworld.h
                 include/perceptron.h
                 include/neuron.h)
set(SOURCES_LIST src/helloworld.cpp
                 src/perceptron.cpp
                 src/neuron.cpp)

include_directories(include)

add_library(
    ${RECOGNITION_LIBRARY_NAME}
    SHARED
    ${SOURCES_LIST}
    ${HEADERS_LIST}
)


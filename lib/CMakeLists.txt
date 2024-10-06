cmake_minimum_required(VERSION 3.28)

project(recognition VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS_LIST include/helloworld.h)
set(SOURCES_LIST src/helloworld.cpp)

include_directories(include)

add_library(
    ${PROJECT_NAME}
    SHARED
    ${SOURCES_LIST}
    ${HEADERS_LIST}
)


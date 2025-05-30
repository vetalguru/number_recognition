cmake_minimum_required(VERSION 3.25)

project(${LIB_RECOGNITION_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HEADERS_LIST ${CMAKE_CURRENT_SOURCE_DIR}/include/neuron.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/include/perceptron.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/include/mnistcsvdataset.hpp)

set(SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/perceptron.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/src/neuron.cpp)

add_library(
    ${LIB_RECOGNITION_NAME}
    SHARED
    ${SOURCES_LIST}
    ${HEADERS_LIST}
)

target_include_directories(
    ${LIB_RECOGNITION_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${THIRDPARTY_DIR}/logger
)

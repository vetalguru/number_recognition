cmake_minimum_required(VERSION 3.25)

project(simpleapp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RECOGNITION_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SOURCES_LIST ${CMAKE_SOURCE_DIR}/main.cpp)

include_directories(${RECOGNITION_LIBRARY_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES_LIST})

add_subdirectory(${RECOGNITION_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} recognition)


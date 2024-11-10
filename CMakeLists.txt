cmake_minimum_required(VERSION 3.25)

project(numrec VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RECOGNITION_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)

set(HEADERS_LIST ${CMAKE_SOURCE_DIR}/application.h)
set(SOURCES_LIST ${CMAKE_SOURCE_DIR}/main.cpp
                 ${CMAKE_SOURCE_DIR}/application.cpp)

include_directories(${RECOGNITION_LIBRARY_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES_LIST} ${HEADERS_LIST})

add_subdirectory(${RECOGNITION_LIBRARY_DIR})
add_subdirectory(${TESTS_DIR})

target_link_libraries(${PROJECT_NAME} recognition)


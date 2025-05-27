cmake_minimum_required(VERSION 3.25)

project(${CMD_RECOGNITION_NAME})

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

set(BUILD_TESTS OFF CACHE BOOL "Disable logger tests" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HEADERS_LIST
    ${CMAKE_BINARY_DIR}/version.h
    ${CMD_RECOGNITION_DIR}/include/application.hpp
)

set(SOURCES_LIST
    ${CMD_RECOGNITION_DIR}/src/main.cpp
    ${CMD_RECOGNITION_DIR}/src/application.cpp
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

message(STATUS "Git command exit status: ${GIT_CMD_STATUS}")
message(STATUS "Git commit hash: '${GIT_COMMIT_HASH}'")
message(STATUS "VERSION BUILD: '${VERSION_BUILD}'")

configure_file(
    ${CMAKE_SOURCE_DIR}/version.h.in
    ${CMAKE_BINARY_DIR}/version.h
    @ONLY
)

find_package(Boost 1.74.0 COMPONENTS program_options json REQUIRED)

add_executable(
    ${CMD_RECOGNITION_NAME}
    ${SOURCES_LIST}
    ${HEADERS_LIST}
)

target_include_directories(
    ${CMD_RECOGNITION_NAME}
    PRIVATE
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${LIB_RECOGNITION_DIR}
    ${CMD_RECOGNITION_DIR}
    ${LIB_RECOGNITION_DIR}/submodule/logger
)

target_link_libraries(
    ${CMD_RECOGNITION_NAME}
    PRIVATE
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

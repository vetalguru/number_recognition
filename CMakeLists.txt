cmake_minimum_required(VERSION 3.25)

project(numrec)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

set(BUILD_TESTS OFF CACHE BOOL "Disable logger tests" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(RECOGNITION_LIBRARY_NAME recognition)
set(RECOGNITION_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)
set(CMD_DIR ${CMAKE_SOURCE_DIR}/cmd)
set(MNIST_ZIP ${CMAKE_SOURCE_DIR}/data/mnist/mnist_datasets_csv.zip)
set(MNIST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/mnist)

set(HEADERS_LIST ${CMAKE_BINARY_DIR}/version.h
                 ${CMD_DIR}/application.h)

set(SOURCES_LIST ${CMD_DIR}/main.cpp
                 ${CMD_DIR}/application.cpp)

execute_process(
  COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(VERSION_BUILD ${GIT_COMMIT_HASH})

configure_file(
  ${CMAKE_SOURCE_DIR}/version.h.in
  ${CMAKE_BINARY_DIR}/version.h
  @ONLY
)

find_package(Boost 1.74.0 COMPONENTS program_options json REQUIRED)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "BOOST LIBRARY NOT FOUND!!!")
endif()

add_subdirectory(${RECOGNITION_LIBRARY_DIR})
add_subdirectory(${RECOGNITION_LIBRARY_DIR}/submodule/logger EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} ${SOURCES_LIST} ${HEADERS_LIST})

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  ${CMAKE_BINARY_DIR}
  ${Boost_INCLUDE_DIRS}
  ${RECOGNITION_LIBRARY_DIR}
  ${CMD_DIR}
  ${RECOGNITION_LIBRARY_DIR}/submodule/logger
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  ${RECOGNITION_LIBRARY_NAME}
  ${Boost_LIBRARIES}
  logger
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

add_subdirectory(${TESTS_DIR})

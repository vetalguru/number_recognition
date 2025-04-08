cmake_minimum_required(VERSION 3.25)

project(number_recognition)

set(LIB_RECOGNITION_NAME recognition)
set(CMD_RECOGNITION_NAME numrec)

set(LIB_RECOGNITION_DIR ${CMAKE_SOURCE_DIR}/lib)
set(CMD_RECOGNITION_DIR ${CMAKE_SOURCE_DIR}/cmd)
set(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)

set(MNIST_ZIP ${CMAKE_SOURCE_DIR}/data/mnist/mnist_datasets_csv.zip)
set(MNIST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/mnist)

add_subdirectory(${LIB_RECOGNITION_DIR})
add_subdirectory(${CMD_RECOGNITION_DIR})
add_subdirectory(${TESTS_DIR})

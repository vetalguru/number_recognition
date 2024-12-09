cmake_minimum_required(VERSION 3.25)

project(numrec)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

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

include_directories(${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RECOGNITION_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)

set(HEADERS_LIST ${CMAKE_BINARY_DIR}/version.h
                 ${CMAKE_SOURCE_DIR}/application.h
                 ${CMAKE_SOURCE_DIR}/mnistdataitem.h
                 ${CMAKE_SOURCE_DIR}/mnistdatareader.h)

set(SOURCES_LIST ${CMAKE_SOURCE_DIR}/main.cpp
                 ${CMAKE_SOURCE_DIR}/application.cpp
                 ${CMAKE_SOURCE_DIR}/mnistdataitem.cpp
                 ${CMAKE_SOURCE_DIR}/mnistdatareader.cpp)

include_directories(${RECOGNITION_LIBRARY_DIR}/include)

find_package(Boost 1.74.0 COMPONENTS program_options)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  set(BOOST_LIBS ${Boost_LIBRARIES})
else()
  set(BOOST_LIBS )
  message(WARNING "BOOST NOT FOUND!!!")
endif()

add_executable(${PROJECT_NAME} ${SOURCES_LIST} ${HEADERS_LIST})

add_subdirectory(${RECOGNITION_LIBRARY_DIR})
add_subdirectory(${TESTS_DIR})

target_link_libraries(${PROJECT_NAME}
    recognition
    ${BOOST_LIBS}
)

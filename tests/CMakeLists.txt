enable_testing()

include(FetchContent)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)

add_executable(
    test_gettestnumber
    test_gettestnumber.cpp
)

target_include_directories(
    test_gettestnumber
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(
    test_gettestnumber
    PRIVATE
    GTest::gtest_main
    ${RECOGNITION_LIBRARY_NAME}
)

add_test(
    NAME test_gettestnumber
    COMMAND test_gettestnumber
)

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

add_executable(test_neuron test_neuron.cpp)
target_include_directories(test_neuron PRIVATE ${LIB_RECOGNITION_DIR})
target_link_libraries(test_neuron PRIVATE GTest::gtest_main ${LIB_RECOGNITION_NAME})

add_executable(test_mnist_csv_dataset test_mnist_csv_dataset.cpp)
target_include_directories(test_mnist_csv_dataset PRIVATE ${LIB_RECOGNITION_DIR})
target_link_libraries(test_mnist_csv_dataset PRIVATE GTest::gtest_main ${LIB_RECOGNITION_NAME})

add_test(NAME test_neuron COMMAND test_neuron)
add_test(NAME test_mnist_csv_dataset COMMAND test_mnist_csv_dataset)

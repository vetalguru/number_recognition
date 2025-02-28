// Copyright 2024 Vitalii Shkibtan

#include "include/helloworld.h"

#include <iostream>

#include "include/logger.hpp"

void PrintTestMessage() {
    std::cout << "Hello world!!!" << std::endl;
}

int GetTestNumber() {
    constexpr int kTestNumber = 42;
    LOG_INFO << "GetTestNumber() called with result: " << kTestNumber;
    return kTestNumber;
}

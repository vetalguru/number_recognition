// Copyright 2024 Vitalii Shkibtan

#include "application.h"

#include <iostream>

#include "lib/include/helloworld.h"

Application::Application(int argc, char* argv[]) {

}

int Application::run() const noexcept {
    PrintTestMessage();

    std::cout << "Test number = " << GetTestNumber() << std::endl;

    return 0;
}

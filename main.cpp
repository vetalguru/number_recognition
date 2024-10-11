// Copyright 2024 Vitalii Shkibtan

#include <iostream>

#include "lib/include/helloworld.h"

int main(int argc, char* argv[]) {
    PrintTestMessage();

    std::cout << "Test number = " << GetTestNumber() << std::endl;

    return 0;
}


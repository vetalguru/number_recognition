// Copyright 2024 Vitalii Shkibtan

#include "lib/include/helloworld.h"

#include <iostream>

int main(int argc, char* argv[]) {
    PrintTestMessage();

    std::cout << "Test number = " << GetTestNumber() << std::endl;

    return 0;
}


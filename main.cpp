// Copyright 2024 Vitalii Shkibtan

#include "./application.h"

#include <iostream>

#include "version.h"

int main(int argc, char* argv[]) {
    std::cout << "Version: " <<
        static_cast<int>(VERSION_MAJOR) << "." <<
        static_cast<int>(VERSION_MINOR) << "." <<
        VERSION_BUILD << std::endl;

    Application app(argc, argv);
    return app.run();
}


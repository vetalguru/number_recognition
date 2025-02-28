// Copyright (c) 2024-2025 Vitalii Shkibtan. All rights reserved.

#include "./application.h"

#include "include/logger.hpp"


int main(int argc, char* argv[]) {
    LOG_DEBUG << "Application started...";
    Application app;
    int result = app.run(argc, argv);
    LOG_DEBUG << "Application finished with code: " << result;
    return result;
}

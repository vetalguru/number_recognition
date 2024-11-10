// Copyright 2024 Vitalii Shkibtan

#include "application.h"

#include <iostream>
#include <string>

#include "lib/include/helloworld.h"

Application::Application(const int argc, const char* const argv[]) {
    parseCommandLine(argc, argv);
}

int Application::run() const noexcept {
    std::cout << '\n' << "Message from library: " << std::endl;
    PrintTestMessage();
    std::cout << '\n' << "Test number = " << GetTestNumber() << std::endl;

    return 0;
}

void Application::parseCommandLine(
    const int argc, const char* const argv[]) const noexcept {
    po::options_description mainDesc{"General options:"};
    std::string taskType;
    mainDesc.add_options()
        ("help,h", "Show help")
        ("mode,m", po::value<std::string>(&taskType),
            "Select mode: training, recognition");

    po::options_description trainDesc("Training options:");
    trainDesc.add_options()
        ("input,i", po::value<std::string>(), "Input file")
        ("info,i", po::value<std::string>(), "Input data file")
        ("output,o", po::value<std::string>(), "Output parameters file");

    po::options_description recDesc("Recognition options:");
    recDesc.add_options()
        ("input,i", po::value<std::string>(), "Input file")
        ("params,p", po::value<std::string>(), "Input parameters file")
        ("output,o", po::value<std::string>(), "Output file");

    po::variables_map vm;
    po::parsed_options parsed =
        po::command_line_parser(argc, argv)
            .options(mainDesc).allow_unregistered().run();

    po::store(parsed, vm);
    po::notify(vm);

    if (taskType == "training") {
        mainDesc.add(trainDesc);
        po::store(po::parse_command_line(argc, argv, mainDesc), vm);
        initTrainingMode(vm);
    } else if (taskType == "recognition") {
        mainDesc.add(recDesc);
        po::store(po::parse_command_line(argc, argv, mainDesc), vm);
        initRecognitionMode(vm);
    } else {
        mainDesc.add(trainDesc).add(recDesc);
        std::cout << mainDesc << std::endl;
    }
}

void Application::initTrainingMode(
    const po::variables_map& vm) const noexcept {
    std::cout << "Detected training mode" << std::endl;

    if (vm.count("input")) {
        std::cout << "input = "
            << vm["input"].as<std::string>() << std::endl;
    }

    if (vm.count("info")) {
        std::cout << "info = "
            << vm["info"].as<std::string>() << std::endl;
    }

    if (vm.count("output")) {
        std::cout << "output = "
            << vm["output"].as<std::string>() << std::endl;
    }
}

void Application::initRecognitionMode(
    const po::variables_map& vm) const noexcept {
    std::cout << "Detected recognition mode" << std::endl;

    if (vm.count("input")) {
        std::cout << "input = "
            << vm["input"].as<std::string>() << std::endl;
    }

    if (vm.count("params")) {
        std::cout << "params = "
            << vm["params"].as<std::string>() << std::endl;
    }

    if (vm.count("output")) {
        std::cout << "output = "
            << vm["output"].as<std::string>() << std::endl;
    }
}

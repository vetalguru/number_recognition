// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "./application.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "boost/program_options.hpp"

// Autogenerated file with current application version
#include "version.h"  // NOLINT (build/include_subdir)

#include "lib/include/helloworld.h"
#include "lib/include/perceptron.h"


// Unnamed namespace to restrict the scope of constants to this translation unit
namespace {
constexpr int kNumClasses = 10;      // Numbers from 0 to 9
constexpr int kImageSize = 28 * 28;  // Images 28 px x 28 px
}

std::string Application::version() const {
    std::ostringstream ss;
    ss << static_cast<int>(VERSION_MAJOR) << "."
       << static_cast<int>(VERSION_MINOR) << "."
       << VERSION_BUILD;

    return ss.str();
}

void Application::parseCommandLine(const int aArgc, const char* const aArgv[]) {
    po::options_description mainDesc{"General options"};
    std::string taskType;

    mainDesc.add_options()
        ("help,h", "Show help message")
        ("version,v", "Show version")
        ("mode,m", po::value<std::string>(&taskType),
            "Select mode: training, recognition");

    po::options_description trainDesc("Training options:");
    trainDesc.add_options()
        ("train-csv-file,t", po::value<std::string>(),
            "Path to train csv file (mnist_train.csv)")
        ("test-csv-file,c", po::value<std::string>(),
            "Path to data file csv (mnist_test.csv)");

    po::options_description recDesc("Recognition options");
    recDesc.add_options()
        ("input,i", po::value<std::string>(), "Input file")
        ("params,p", po::value<std::string>(), "Input parameters file")
        ("output,o", po::value<std::string>(), "Output file");

    mainDesc.add(trainDesc).add(recDesc);

    po::variables_map vm;
    po::store(po::parse_command_line(aArgc, aArgv, mainDesc), vm);
    po::notify(vm);

    if (vm.count("version")) {
        std::cout << version() << std::endl;
        return;
    }

    if (vm.count("help") || taskType.empty()) {
        std::cout << mainDesc << std::endl;
        return;
    }

    if (taskType == "training") {
        initTrainingMode(vm);
    } else if (taskType == "recognition") {
        initRecognitionMode(vm);
    } else {
        std::cout << "Error: unknown mode '" << taskType << "'" << std::endl;
        std::cout << mainDesc << std::endl;
    }
}

void Application::initTrainingMode(const po::variables_map& aVm) {
    std::cout << "Training mode parameters:" << std::endl;

    std::string training_file;
    std::string test_file;

    if (aVm.count("train-csv-file")) {
        training_file = aVm["train-csv-file"].as<std::string>();
        std::cout << "Train file name: " << training_file << std::endl;
    }

    if (aVm.count("test-csv-file")) {
        test_file = aVm["test-csv-file"].as<std::string>();
        std::cout << "Test file name: "<< test_file << std::endl;
    }

    handleTrainingMode(training_file, test_file);
}

void Application::initRecognitionMode(const po::variables_map& aVm) {
    std::cout << "Detected recognition mode" << std::endl;

    if (aVm.count("input")) {
        std::cout << "input = "
            << aVm["input"].as<std::string>() << std::endl;
    }

    if (aVm.count("params")) {
        std::cout << "params = "
            << aVm["params"].as<std::string>() << std::endl;
    }

    if (aVm.count("output")) {
        std::cout << "output = "
            << aVm["output"].as<std::string>() << std::endl;
    }
}

bool Application::loadMnistCsv(const std::string& aFileName,
    std::vector<std::vector<double>>& aInputs,
    std::vector<std::vector<double>>& aTargets) const {

    constexpr char kDelimeter = ',';

    std::ifstream file(aFileName);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " + aFileName << std::endl;
        return false;
    }

    std::string line;

    // First line in csv is a header, pass it
    if (!std::getline(file, line)) {
        std::cerr << "Error: File " << aFileName
            << " has wrong format" << std::endl;
        return false;
    }

    int lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;

        std::stringstream ss(line);
        std::vector<double> pixels(kImageSize, 0.0);
        std::vector<double> labels(kNumClasses, 0.0);

        std::string cell;
        int label = -1;
        // Get label (first value)
        if (!std::getline(ss, cell, kDelimeter)) {
            std::cerr << "Error: Missing label in line " << lineNumber
                << " of file " << aFileName << std::endl;
            return false;
        }

        try {
            label = std::stoi(cell);
            if (label < 0 || label >= kNumClasses) {
                std::cerr << "Error: Label in line " << lineNumber
                    << " is out of valid range in file "
                    << aFileName << std::endl;
                return false;
            }
            labels[label] = 1.0;  // One-hot encoding
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid label '" << cell
                << "' in line " << lineNumber << " of file " << aFileName
                << " with error: " << e.what() << std::endl;
            return false;
        }

        // Get image pixels
        size_t pixelCounter = 0;
        while (pixelCounter < kImageSize &&
               std::getline(ss, cell, kDelimeter)) {
            try {
                pixels[pixelCounter++] =
                    std::stod(cell) / 255.0;  // Pixel normalization
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid pixel value '" << cell
                    << "' line " << lineNumber
                    << " in column " << pixelCounter + 1
                    << " in file "<< aFileName
                    << " with error: " << e.what() << std::endl;
                return false;
            }
        }

        if (pixelCounter != kImageSize) {
            std::cerr << "Error: Line " << lineNumber
                << " in file " << aFileName << " has " << pixelCounter
                << "pixels values but expected " << kImageSize << std::endl;
            return false;
        }

        aInputs.emplace_back(std::move(pixels));
        aTargets.emplace_back(std::move(labels));
    }

    if (aInputs.empty() || aInputs.size() != aTargets.size()) {
        std::cerr << "Error: No valid data found in file "
            << aFileName << std::endl;
        aInputs.clear();
        aTargets.clear();
        return false;
    }

    return true;
}

int Application::run(const int aArgc, const char* const aArgv[]) {
    parseCommandLine(aArgc, aArgv);

    return 0;
}

void Application::handleTrainingMode(const std::string& aMnistTrainFile,
        const std::string& aMnistTestFile) {
    // Epochs   Speed           Accuracy
    // 10       🏎 fast         ~97% (optimal)
    // 15       🐢 slowly       ~98%
    constexpr int kEpoch = 15;

    /**
       Learning rate    Speed                 Accuracy    Problems
       0.01             🚀 fast            ✅ ~98%     Optimal
       0.05             🏎 very fast        ⚠ ~97%     Fluctuations
       0.1              ⚠  fast            ❌ ~90%     Unstable
    */
    constexpr double kLearningRate = 0.0075;

    /**
       Net architecture       Speed       Accuracy
       784 → 128 → 10         🚀 fast     97-98%
       784 → 256 → 128 → 10   ⚡ medium   98-99%
    */
    NeuroNetwork network({kImageSize, 256, 128, kNumClasses});

    // Load train data
    std::vector<std::vector<double>> trainInputs;
    std::vector<std::vector<double>> trainTargets;
    if (!loadMnistCsv(aMnistTrainFile, trainInputs, trainTargets)) {
        std::cerr << "Error: Unable to load MNIST data from file "
            << aMnistTrainFile << std::endl;
        return;
    }

    // Train model
    std::cout << "Training started..." << std::endl;
    network.train(trainInputs, trainTargets, kEpoch, kLearningRate);
    std::cout << "Training finished" << std::endl;

    // Load test data
    std::vector<std::vector<double>> testInputs;
    std::vector<std::vector<double>> testTargets;
    if (!loadMnistCsv(aMnistTestFile, testInputs, testTargets)) {
        std::cerr << "Error: Unable to load MNIST data from file "
            << aMnistTestFile << std::endl;
    }

    int correct = 0;
    for (size_t i = 0; i < testInputs.size(); ++i) {
        std::vector<double> output =
            network.forward(testInputs[i]).back();

        // Get predicted number (Max number from predicted)
        int predicted = std::distance(output.begin(),
            std::max_element(output.begin(), output.end()));
        int actual = std::distance(testTargets[i].begin(),
            std::max_element(testTargets[i].begin(),
            testTargets[i].end()));

        if (predicted == actual) {
            ++correct;
        }
    }

    std::cout << "Accuracy: " << (correct * 100.0 / testInputs.size())
        << "%" << std::endl;
}

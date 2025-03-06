// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "cmd/application.h"

#include <algorithm>
#include <iostream>  // For help and version output
#include <filesystem>
#include <fstream>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "boost/program_options.hpp"

// Autogenerated file with current application version
#include "version.h"  // NOLINT (build/include_subdir)

#include "boost/json.hpp"

#include "include/logger.hpp"
#include "lib/include/helloworld.h"


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
        ("train-data,t", po::value<std::string>(),
            "Path to train csv file (mnist_train.csv)")
        ("test-data,c", po::value<std::string>(),
            "Path to data file csv (mnist_test.csv)")
        ("save-model,s", po::value<std::string>(),
            "Output file with trained model and network configuration");

    po::options_description recDesc("Recognition options");
    recDesc.add_options()
        ("data,d", po::value<std::string>(),
            "Path to file with data to recognize")
        ("model,p", po::value<std::string>(),
            "Path to file with learned model")
        ("result,r", po::value<std::string>(),
            "Output file with recognition results");

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
        LOG_ERROR << "Unknown mode. Valid modes are 'training'"
            << " and 'recognition'.";
    }
}

void Application::initTrainingMode(const po::variables_map& aVm) {
    LOG_INFO << "Training mode parameters:";

    std::string trainingFile;
    std::string testFile;
    std::string outputModelFile;

    if (aVm.count("train-data")) {
        try {
            trainingFile = aVm["train-data"].as<std::string>();
        } catch (const std::exception& e) {
            LOG_ERROR << "Wrong -- train-data parameter format. Error: "
                << e.what();
                return;
        }
        LOG_INFO << "Train file name: " << trainingFile;
    } else {
        LOG_ERROR << "Missing train file";
        return;
    }

    if (aVm.count("test-data")) {
        try {
            testFile = aVm["test-data"].as<std::string>();
        } catch (const std::exception& e) {
            LOG_ERROR << "Wrong --test-data parameter format. Error: "
                << e.what();
            return;
        }
        LOG_INFO << "Test file name: "<< testFile;
    } else {
        LOG_ERROR << "Missing test file";
        return;
    }

    if (aVm.count("save-model")) {
        try {
            outputModelFile = aVm["save-model"].as<std::string>();
        } catch (const std::exception& e) {
            LOG_ERROR << "Wrong --save-model parameter format. Error: "
                << e.what();
            return;
        }
        LOG_INFO << "Output model file: " << outputModelFile;
    } else {
        LOG_ERROR << "Output model file is not specified";
        return;
    }

    handleTrainingMode(trainingFile, testFile, outputModelFile);
}

void Application::initRecognitionMode(const po::variables_map& aVm) {
    LOG_INFO << "Detected recognition mode";

    std::string dataFile;
    std::string modelFile;
    std::string resultFile;
    if (aVm.count("data")) {
        try {
            dataFile = aVm["data"].as<std::string>();
        } catch (const std::exception& e) {
            LOG_ERROR << "Wrong --data parameter format. Error: "
                << e.what();
            return;
        }
        LOG_INFO << "Data file:" << dataFile;
    } else {
        LOG_ERROR << "Data file is not specified";
        return;
    }

    if (aVm.count("model")) {
        try {
            modelFile = aVm["model"].as<std::string>();
        } catch (const std::exception& e) {
            LOG_ERROR << "Wrong --model parameter format. Error: "
                << e.what();
            return;
        }
        LOG_INFO << "Model file: " << modelFile;
    } else {
        LOG_ERROR << "File with model is not specified";
        return;
    }

    if (aVm.count("result")) {
        try {
            resultFile = aVm["result"].as<std::string>();
        } catch (const std::exception& e) {
            LOG_ERROR << "Wrong --result parameter format. Error: "
                << e.what();
            return;
        }
        LOG_INFO << "Result file: " << resultFile;
    } else {
        LOG_ERROR << "Result file path is not specified";
        return;
    }

    handleRecognitionMode(dataFile, modelFile, resultFile);
}

bool Application::loadMnistCsv(const std::string& aFileName,
    std::vector<std::vector<double>>& aInputs,
    std::vector<std::vector<double>>& aTargets) const {

    if (!std::filesystem::exists(aFileName)) {
        LOG_ERROR << "File " << aFileName << " does not exist";
        return false;
    }

    constexpr char kDelimeter = ',';

    std::ifstream file(aFileName);
    if (!file.is_open()) {
        LOG_ERROR << "Unable to open file: " + aFileName;
        return false;
    }

    std::string line;

    // First line in csv is a header, pass it
    if (!std::getline(file, line)) {
        LOG_ERROR << "File " << aFileName << " has wrong format";
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
            LOG_ERROR << "Missing label in line " << lineNumber
                << " of file " << aFileName;
            return false;
        }

        try {
            label = std::stoi(cell);
            if (label < 0 || label >= kNumClasses) {
                LOG_ERROR << "Label in line " << lineNumber
                    << " is out of valid range in file " << aFileName;
                return false;
            }
            labels[label] = 1.0;  // One-hot encoding
        } catch (const std::exception& e) {
            LOG_ERROR << "Invalid label '" << cell
                << "' in line " << lineNumber << " of file " << aFileName
                << " with error: " << e.what();
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
                LOG_ERROR << "Invalid pixel value '" << cell
                    << "' line " << lineNumber
                    << " in column " << pixelCounter + 1
                    << " in file "<< aFileName
                    << " with error: " << e.what();
                return false;
            }
        }

        if (pixelCounter != kImageSize) {
            LOG_ERROR << "Line " << lineNumber
                << " in file " << aFileName << " has " << pixelCounter
                << "pixels values but expected " << kImageSize;
            return false;
        }

        aInputs.emplace_back(std::move(pixels));
        aTargets.emplace_back(std::move(labels));
    }

    if (aInputs.empty() || aInputs.size() != aTargets.size()) {
        LOG_ERROR << "No valid data found in file " << aFileName;
        aInputs.clear();
        aTargets.clear();
        return false;
    }

    return true;
}

bool Application::saveModelToJson(const std::string& aFileName,
    const Perceptron& aNetwork) const {
    if (aFileName.empty()) {
        LOG_ERROR << "Empty JSON file name";
        return false;
    }

    if (std::filesystem::exists(aFileName)) {
        LOG_ERROR << "File " << aFileName << " already exists";
        return false;
    }

    if (!aNetwork.isConfigured()) {
        LOG_ERROR << "Network is not configured";
        return false;
    }

    if (!aNetwork.isTrained()) {
        LOG_ERROR << "Network is not trained";
        return false;
    }

    boost::json::object jsonModel;
    jsonModel["architecture"] = boost::json::array();
    jsonModel["layers"] = boost::json::array();

    // Fill architecture
    try {
        // Add first (input) layer
        if (!aNetwork.layers().empty()) {
            jsonModel["architecture"].as_array().emplace_back(kImageSize);
        }

        for (const auto& layer : aNetwork.layers()) {
            jsonModel["architecture"].as_array().emplace_back(
                static_cast<int>(layer.size()));
        }
    } catch (const std::exception& e) {
        LOG_ERROR << "Unable to fill JSON architecture with error " << e.what();
        return false;
    }

    // Fill layers and weights
    try {
        for (const auto& layer : aNetwork.layers()) {
            boost::json::object layerJson;
            layerJson["neurons"] = boost::json::array();

            for (const auto& neuron : layer) {
                boost::json::object neuronJson;
                neuronJson["bias"] = neuron.bias();
                neuronJson["weights"] = boost::json::array();

                for (const auto& weight : neuron.weights()) {
                    neuronJson["weights"].as_array().emplace_back(weight);
                }

                layerJson["neurons"].as_array().emplace_back(neuronJson);
            }

            jsonModel["layers"].as_array().emplace_back(layerJson);
        }
    } catch (const std::exception& e) {
        LOG_ERROR << "Unable to fill layers and biases to JSON with error"
            << e.what();
        return false;
    }

    // Save to JSON
    std::ofstream file(aFileName);
    if (!file.is_open()) {
        LOG_ERROR << "Unable to open file " << aFileName;
        return false;
    }

    try {
        file << boost::json::serialize(jsonModel) << std::endl;
    } catch (const std::exception& e) {
        LOG_ERROR << "Unable to write JSON to the file " << aFileName
            << " with error " << e.what();
        return false;
    }

    file.close();

    LOG_INFO << "Model saved to " << aFileName;

    return true;
}

bool Application::loadModelFromJson(const std::string& aFileName,
    Perceptron& aNetwork) const {
    if (aFileName.empty()) {
        LOG_ERROR << "Empty JSON file name";
        return false;
    }

    if (!std::filesystem::exists(aFileName)) {
        LOG_ERROR << "File " << aFileName << " does not exist";
        return false;
    }

    std::ifstream file(aFileName);
    if (!file.is_open()) {
        LOG_ERROR << "Unable to open file " << aFileName;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    boost::json::value parsedJson;
    try {
        parsedJson = boost::json::parse(buffer.str());
    } catch (const std::exception& e) {
        LOG_ERROR << "Unable to parse JSON from file "
            << aFileName << " with error: " << e.what();
        return false;
    }

    boost::json::object jsonModel = parsedJson.as_object();
    // Check if JSON has required fields and types
    if (!jsonModel.contains("architecture") ||
        !jsonModel["architecture"].is_array() ||
        !jsonModel.contains("layers") ||
        !jsonModel["layers"].is_array()) {
        LOG_ERROR << "Invalid JSON format in file " << aFileName;
        return false;
    }

    // Read architecture
    std::vector<int> architecture;
    try {
        for (const auto& layerSize : jsonModel["architecture"].as_array()) {
            if (!layerSize.is_number()) {
                LOG_ERROR << "Invalid layer size in JSON file " << aFileName;
                return false;
            }

            architecture.emplace_back(layerSize.as_int64());
        }
    }
    catch(const std::exception& e) {
        LOG_ERROR << "Unable to read architecture from file "
            << aFileName << " with error: " << e.what();
        return false;
    }

    if (architecture.empty()) {
        LOG_ERROR << "Wrong network architecture in file " << aFileName;
        return false;
    }

    // Set architecture
    if (!aNetwork.initializeNetwork(architecture)) {
        LOG_ERROR << "Unable to configure network";
        return false;
    }

    const auto& jsonLayers = jsonModel["layers"].as_array();
    if (jsonLayers.size() != architecture.size() - 1) {
        LOG_ERROR
            << "Mismatch between architecture and number of layers in JSON";
        return false;
    }

    // Read weights and biases
    try {
        for (size_t layerIndex = 0; layerIndex < jsonLayers.size();
            ++layerIndex) {
            const auto& jsonLayer = jsonLayers[layerIndex];
            for (size_t neuronIndex = 0;
                    neuronIndex <
                        jsonLayer.at("neurons").as_array().size();
                    ++neuronIndex) {
                const auto& neuronJson =
                    jsonLayer.at("neurons").as_array()[neuronIndex];

                const auto& weightsJson =
                    neuronJson.at("weights").as_array();
                std::vector<double> weights;
                for (const auto& weight : weightsJson) {
                    weights.emplace_back(weight.as_double());
                }

                aNetwork.setNeuronWeights(layerIndex, neuronIndex, weights);
                aNetwork.setNeuronBias(layerIndex, neuronIndex,
                    neuronJson.at("bias").as_double());
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR << "Unable to read weights and biases from file "
            << aFileName << " with error " << e.what();
        return false;
    }

    LOG_INFO << "Model successfully loaded from " << aFileName;
    return true;
}

int Application::run(const int aArgc, const char* const aArgv[]) {
    parseCommandLine(aArgc, aArgv);
    return EXIT_SUCCESS;
}

void Application::handleTrainingMode(const std::string& aMnistTrainFile,
        const std::string& aMnistTestFile,
        const std::string& aOutputModelFile) {
    if (!std::filesystem::exists(aMnistTrainFile)) {
        LOG_ERROR<< "Train file " << aMnistTrainFile << " does not exist";
        return;
    }

    if (!std::filesystem::exists(aMnistTestFile)) {
        LOG_ERROR << "Test file " << aMnistTestFile << "does not exist";
        return;
    }

    if (std::filesystem::exists(aOutputModelFile)) {
        LOG_ERROR << "Output file " << aOutputModelFile << " already exists";
        return;
    }

    // Epochs   Speed           Accuracy
    // 10       🏎 fast         ~97% (optimal)
    // 15       🐢 slowly       ~98%
    constexpr int kEpoch = 10;

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

    LOG_INFO << "Epoches: " << kEpoch;
    LOG_INFO << "Learning rate: " << kLearningRate;
    Perceptron network({kImageSize, 256, 128, kNumClasses});

    // Load train data
    std::vector<std::vector<double>> trainInputs;
    std::vector<std::vector<double>> trainTargets;
    if (!loadMnistCsv(aMnistTrainFile, trainInputs, trainTargets)) {
        LOG_ERROR << "Unable to load MNIST data from file " << aMnistTrainFile;
        return;
    }

    // Train model
    LOG_INFO << "Training started...";
    network.train(trainInputs, trainTargets, kEpoch, kLearningRate);
    LOG_INFO << "Training finished";

    // Load test data
    std::vector<std::vector<double>> testInputs;
    std::vector<std::vector<double>> testTargets;
    if (!loadMnistCsv(aMnistTestFile, testInputs, testTargets)) {
        LOG_ERROR << "Unable to load MNIST data from file " << aMnistTestFile;
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

    LOG_INFO << "Accuracy: " << (correct * 100.0 / testInputs.size()) << "%";

    // Save model to JSON
    if (!saveModelToJson(aOutputModelFile, network)) {
        LOG_ERROR << "Unable to save model to JSON";
    } else {
        LOG_INFO << "Model saved to model.json";
    }
}

void Application::handleRecognitionMode(const std::string& aDataFile,
    const std::string& aModelFile, const std::string& aResultFile) {
    LOG_INFO << "Recognition started...";

    if (!std::filesystem::exists(aModelFile)) {
        LOG_ERROR << "Model file " << aModelFile << " does not exist";
        return;
    }

    if (!std::filesystem::exists(aDataFile)) {
        LOG_ERROR << "Data file " << aDataFile << " does not exist";
        return;
    }

    // Load model
    Perceptron network;
    if (!loadModelFromJson(aModelFile, network)) {
        LOG_ERROR << "Failed to load model from " << aModelFile;
        return;
    }

    // Load data
    std::vector<std::vector<double>> inputData;
    std::vector<std::vector<double>> dummyTarget;
    if (!loadMnistCsv(aDataFile, inputData, dummyTarget)) {
        LOG_ERROR << "Failed to load data from " << aDataFile;
        return;
    }

    double minPixel =
        *std::min_element(inputData[0].begin(), inputData[0].end());
    double maxPixel =
        *std::max_element(inputData[0].begin(), inputData[0].end());
    LOG_INFO << "Pixel value range: [" << minPixel << ", " << maxPixel << "]";

    std::ofstream resultFile(aResultFile);
    if (!resultFile.is_open()) {
        LOG_ERROR << "Unable to create result file " << aResultFile;
        return;
    }

    // Recognize
    size_t matches = 0;
    for (size_t i = 0; i < inputData.size(); ++i) {
        std::vector<double> output = network.forward(inputData[i]).back();

        auto maxPredictElementIter =
            std::max_element(output.begin(), output.end());
        int predictedClass =
            std::distance(output.begin(), maxPredictElementIter);

        int expectedClass = -1;
        if (!dummyTarget[i].empty()) {
            auto maxExpectedElementIter =
                std::max_element(dummyTarget[i].begin(), dummyTarget[i].end());
            expectedClass =
                std::distance(dummyTarget[i].begin(), maxExpectedElementIter);
        } else {
            LOG_ERROR << "Empty target at index " << i;
            continue;
        }

        // Write result to file
        resultFile << "Expected: " << expectedClass
            << "\tPredicted: " << predictedClass << std::endl;

        if (expectedClass == predictedClass) {
            ++matches;
        }
    }

    LOG_INFO << "Matches: " << matches << " of " << inputData.size();
    LOG_INFO << "Recognition accuracy: " <<
        (matches * 100.0 / inputData.size()) << "%";
    LOG_INFO << "Recognition completed. Result saved to file " << aResultFile;
}

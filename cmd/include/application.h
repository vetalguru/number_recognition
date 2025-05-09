// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef CMD_INCLUDE_APPLICATION_H_
#define CMD_INCLUDE_APPLICATION_H_

#include <string>
#include <vector>

#include "include/perceptron.h"


namespace boost {
namespace program_options {
class variables_map;
}  // namespace program_options
}  // namespace boost

namespace po = boost::program_options;

class Application {
 public:
    static constexpr size_t kNumClasses = 10;      // Numbers from 0 to 9
    static constexpr size_t kImageSize = 28 * 28;  // Images 28 px x 28 px
    static constexpr char kDefaultHiddenLayers[] = "512";
    static constexpr int kDefaultEpochs = 40;
    static constexpr double kDefaultLearningRate = 0.001;

    static constexpr char kMnistCsvDelimeter = ',';

 public:
    Application() = default;
    ~Application() = default;

    int run(const int aArgc, const char* const aArgv[]);

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

 private:
    std::string version() const;

    void parseCommandLine(const int aArgc, const char* const aArgv[]);
    void initTrainingMode(const po::variables_map& aVm);
    void initRecognitionMode(const po::variables_map& aVm);

    void handleTrainingMode(
        const std::string& aMnistTrainFile,
        const std::string& aMnistTestFile,
        const std::string& aOutputModelFile,
        const std::vector<size_t> aLayers,
        const int aEpochs,
        const double aLearningRate);

    void handleRecognitionMode(
        const std::string& aDataFile,
        const std::string& aModelFile,
        const std::string& aResultFile);

    bool loadMnistCsv(
        const std::string& aFileName,
        // NOLINTNEXTLINE(runtime/references)
        std::vector<std::vector<double>>& aInputs,
        // NOLINTNEXTLINE(runtime/references)
        std::vector<std::vector<double>>& aTargets) const;

    bool saveModelToJson(
        const std::string& aFileName,
        const Perceptron& aNetwork) const;

    bool loadModelFromJson(
        const std::string& aFileName,
        Perceptron& aNetwork) const;  // NOLINT(runtime/references)

    template <typename T>
    bool getValue(const po::variables_map& aVm, const std::string& aKey,
        // NOLINTNEXTLINE(runtime/references)
        T& aOut, const std::string& aLabel);

    std::vector<size_t> parseLayersString(
        const std::string& aInput,
        size_t aImageSize = kImageSize,
        size_t aNumClasses = kNumClasses) const;

    std::string vectorToString(const std::vector<size_t>& aVector,
                               char delimiter = ',') const;
};

#endif  // CMD_INCLUDE_APPLICATION_H_

// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef CMD_INCLUDE_APPLICATION_HPP_
#define CMD_INCLUDE_APPLICATION_HPP_

#include <string>
#include <vector>

#include "include/perceptron.hpp"
#include "include/mnistcsvdataset.hpp"

namespace boost {
namespace program_options {
class variables_map;
}  // namespace program_options
}  // namespace boost

namespace po = boost::program_options;

class Application {
 public:
    Application() = default;
    ~Application() = default;

    int run(const int aArgc, const char* const aArgv[]) const;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

 private:
    std::string version() const;

    void parseCommandLine(const int aArgc, const char* const aArgv[]) const;
    void initTrainingMode(const po::variables_map& aVm) const;
    void initRecognitionMode(const po::variables_map& aVm) const;

    void handleTrainingMode(
        const std::string& aMnistTrainFile,
        const std::string& aMnistTestFile,
        const std::string& aOutputModelFile,
        const std::vector<size_t>& aLayers,
        const int aEpochs,
        const double aLearningRate) const;

    void handleRecognitionMode(
        const std::string& aDataFile,
        const std::string& aModelFile,
        const std::string& aResultFile) const;

    bool saveModelToJson(
        const std::string& aFileName,
        const Perceptron& aNetwork) const;

    bool loadModelFromJson(
        const std::string& aFileName,
        Perceptron& aNetwork) const;  // NOLINT(runtime/references)

    template <typename T>
    bool getValue(const po::variables_map& aVm, const std::string& aKey,
        // NOLINTNEXTLINE(runtime/references)
        T& aOut, const std::string& aLabel) const;

    std::vector<size_t> parseLayersString(
        const std::string& aInput,
        size_t aImageSize = kImageSize,
        size_t aNumClasses = kNumClasses) const;

    std::string vectorToString(const std::vector<size_t>& aVector,
                               char delimiter = ',') const;

    std::vector<double> toOneHot(uint8_t aLabel, size_t aNumClasses = 10) const;

    std::vector<double> normalizeImage(
        const MnistCsvDataSet::Image_t& image) const;

    static constexpr int kNumClasses = 10;      // Numbers from 0 to 9
    static constexpr int kImageSize = 28 * 28;  // Images 28 px x 28 px
};

#endif  // CMD_INCLUDE_APPLICATION_HPP_

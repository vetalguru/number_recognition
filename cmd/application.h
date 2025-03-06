// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef CMD_APPLICATION_H_
#define CMD_APPLICATION_H_

#include <string>
#include <vector>

#include "../lib/include/perceptron.h"


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

    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    int run(const int aArgc, const char* const aArgv[]);

 private:
    std::string version() const;

    void parseCommandLine(const int aArgc, const char* const aArgv[]);
    void initTrainingMode(const po::variables_map& aVm);
    void initRecognitionMode(const po::variables_map& aVm);

    void handleTrainingMode(
        const std::string& aMnistTrainFile,
        const std::string& aMnistTestFile,
        const std::string& aOutputModelFile);

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
        Perceptron& aNetwork) const;
};

#endif  // CMD_APPLICATION_H_

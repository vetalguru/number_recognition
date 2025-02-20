// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <string>
#include <vector>


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

    Application& operator=(Application const&) = delete;

    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    int run(const int aArgc, const char* const aArgv[]);

    std::string version() const;

    void parseCommandLine(const int aArgc, const char* const aArgv[]);
    void initTrainingMode(const po::variables_map& aVm);
    void initRecognitionMode(const po::variables_map& aVm);

    void handleTrainingMode(const std::string& aMnistTrainFile,
        const std::string& aMnistTestFile);

    bool loadMnistCsv(const std::string& aFileName,
        // NOLINTNEXTLINE(runtime/references)
        std::vector<std::vector<double>>& aInputs,
        // NOLINTNEXTLINE(runtime/references)
        std::vector<std::vector<double>>& aTargets) const;
};

#endif  // APPLICATION_H_

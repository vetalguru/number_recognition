// Copyright 2024 Vitalii Shkibtan

#ifndef APPLICATION_H
#define APPLICATION_H

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Application
{
public:
    explicit Application(const int argc, const char* const argv[]);
    ~Application() = default;

    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

    Application(Application&&) = delete;
    Application&& operator=(Application&&) = delete;

    int run() const noexcept;

private:
    void parseCommandLine(const int argc, const char* const argv[]) const noexcept;
    void initTrainingMode(const po::variables_map& vm) const noexcept;
    void initRecognitionMode(const po::variables_map& vm) const noexcept;
};

#endif // APPLICATION_H

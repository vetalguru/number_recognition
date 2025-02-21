// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef LIB_INCLUDE_LOGGER_H_
#define LIB_INCLUDE_LOGGER_H_

#include <chrono>   // NOLINT(build/c++11)
#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>    // NOLINT(build/c++11)
#include <string>
#include <sstream>
#include <iomanip>


class Logger{
 public:
    enum class LogLevel { TRACE, DEBUG, INFO, WARNING, ERROR, FATAL };

 public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    bool setLogFile(const std::string& aFileName) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_logFile = std::make_unique<std::ofstream>(aFileName, std::ios::app);
        if (!m_logFile || !m_logFile->is_open()) {
            m_logFile.reset();
            return false;
        }

        return true;
    }

    void log(LogLevel aLevel, const std::string& aMessage) {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string logMessage = formatMessage(aLevel, aMessage);

        // Print to console
        if (aLevel == LogLevel::ERROR || aLevel == LogLevel::FATAL) {
            std::cerr << logMessage << std::endl;
        } else {
            std::cout << logMessage << std::endl;
        }

        // Print to file
        if (m_logFile && m_logFile->is_open()) {
            *m_logFile << logMessage << std::endl;
            m_logFile->flush();
        }
    }

    class LogStream {
     public:
        explicit LogStream(Logger::LogLevel aLevel) : m_level(aLevel) {}
        ~LogStream() { Logger::getInstance().log(m_level, m_buffer.str()); }

        template<typename T>
        LogStream& operator<<(const T& aValue) {
            m_buffer << aValue;
            return *this;
        }

     private:
        LogLevel m_level;
        std::ostringstream m_buffer;
    };

    static void trace_msg(const std::string& aMsg) {
        getInstance().log(LogLevel::TRACE, aMsg);
    }

    static void debug_msg(const std::string& aMsg) {
        getInstance().log(LogLevel::DEBUG, aMsg);
    }

    static void info_msg(const std::string& aMsg) {
        getInstance().log(LogLevel::INFO, aMsg);
    }

    static void warning_msg(const std::string& aMsg) {
        getInstance().log(LogLevel::WARNING, aMsg);
    }

    static void error_msg(const std::string& aMsg) {
        getInstance().log(LogLevel::ERROR, aMsg);
    }

    static void fatal_msg(const std::string& aMsg) {
        getInstance().log(LogLevel::FATAL, aMsg);
    }

    static LogStream trace()    { return LogStream(LogLevel::TRACE); }
    static LogStream debug()    { return LogStream(LogLevel::DEBUG); }
    static LogStream info()     { return LogStream(LogLevel::INFO); }
    static LogStream warning()  { return LogStream(LogLevel::WARNING); }
    static LogStream error()    { return LogStream(LogLevel::ERROR); }
    static LogStream fatal()    { return LogStream(LogLevel::FATAL); }

 private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    std::string logLevelToString(LogLevel aLevel) {
        std::string result;
        switch (aLevel) {
            case LogLevel::TRACE: result = "TRACE"; break;
            case LogLevel::DEBUG: result = "DEBUG"; break;
            case LogLevel::INFO: result = "INFO"; break;
            case LogLevel::WARNING: result = "WARNING"; break;
            case LogLevel::ERROR: result = "ERROR"; break;
            case LogLevel::FATAL: result = "FATAL"; break;
            default: result = "UNKNOWN"; break;
        }
        return result;
    }

    std::string getCurrentTime() {
        using std::chrono::system_clock;
        using std::chrono::duration_cast;
        using std::chrono::microseconds;

        auto now = system_clock::now();
        auto time = system_clock::to_time_t(now);
        auto msec = duration_cast<microseconds>(
            now.time_since_epoch()) % 1'000'000;

        std::tm localTime{};
        localtime_r(&time, &localTime);

        std::ostringstream ts;
        ts << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S")
            << "." << std::setfill('0') << std::setw(6) << msec.count();
        return ts.str();
    }

    std::string formatMessage(LogLevel aLevel,
        const std::string& aMessage) {
        std::ostringstream output;
        output << "[" << getCurrentTime() << "] ["
            << logLevelToString(aLevel) << "]\t" << aMessage;
        return output.str();
    }

 private:
    std::mutex m_mutex;
    std::unique_ptr<std::ofstream> m_logFile;
};

#define LOG_TRACE_MSG(aMsg)     Logger::trace_msg(aMsg)
#define LOG_DEBUG_MSG(aMsg)     Logger::debug_msg(aMsg)
#define LOG_INFO_MSG(aMsg)      Logger::info_msg(aMsg)
#define LOG_WARNING_MSG(aMsg)   Logger::warning_msg(aMsg)
#define LOG_ERROR_MSG(aMsg)     Logger::error_msg(aMsg)
#define LOG_FATAL_MSG(aMsg)     Logger::fatal_msg(aMsg)

#define LOG_TRACE       Logger::trace()
#define LOG_DEBUG       Logger::debug()
#define LOG_INFO        Logger::info()
#define LOG_WARNING     Logger::warning()
#define LOG_ERROR       Logger::error()
#define LOG_FATAL       Logger::fatal()

#endif  // LIB_INCLUDE_LOGGER_H_

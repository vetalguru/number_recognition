// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef LIB_INCLUDE_MNISTCSVDATASET_HPP_
#define LIB_INCLUDE_MNISTCSVDATASET_HPP_

#include <array>
#include <fstream>
#include <mutex>  // NOLINT(build/c++11)
#include <shared_mutex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class MnistCsvDataSet final {
 public:
    static constexpr uint8_t kMnistImageWidth = 28;
    static constexpr uint8_t kMnistImageHeight = 28;
    static constexpr uint16_t kMnistImageSize =
        kMnistImageWidth * kMnistImageHeight;
    static constexpr char kMnistCsvDelimiter = ',';

    using Image = std::array<uint8_t, kMnistImageSize>;
    using Label = uint8_t;
    using Pixel = uint8_t;
    using Entry = std::pair<Label, Image>;
    using container_type = std::vector<Entry>;

    explicit MnistCsvDataSet(const std::string& aCsvPath) {
        m_isLoaded = loadCsv(aCsvPath);
    }

    MnistCsvDataSet(const MnistCsvDataSet& aOther) {
        std::shared_lock lock(aOther.m_mutex);
        m_data = aOther.m_data;
        m_isLoaded = aOther.m_isLoaded;
    }

    MnistCsvDataSet& operator=(const MnistCsvDataSet& aOther) {
        if (this != &aOther) {
            std::scoped_lock lock(m_mutex, aOther.m_mutex);
            m_data = aOther.m_data;
            m_isLoaded = aOther.m_isLoaded;
        }
        return *this;
    }

    MnistCsvDataSet(MnistCsvDataSet&& aOther) noexcept {
        std::unique_lock lock(aOther.m_mutex);
        m_data = std::move(aOther.m_data);
        m_isLoaded = aOther.m_isLoaded;
    }

    MnistCsvDataSet& operator=(MnistCsvDataSet&& aOther) noexcept {
        if (this != &aOther) {
            std::scoped_lock lock(m_mutex, aOther.m_mutex);
            m_data = std::move(aOther.m_data);
            m_isLoaded = aOther.m_isLoaded;
        }
        return *this;
    }

    ~MnistCsvDataSet() = default;

    std::size_t size() const noexcept {
        std::shared_lock lock(m_mutex);
        return m_data.size();
    }

    const Entry& operator[](std::size_t aIndex) const noexcept {
        std::shared_lock lock(m_mutex);
        return m_data[aIndex];
    }

    const Entry& at(std::size_t aIndex) const {
        std::shared_lock lock(m_mutex);
        return m_data.at(aIndex);
    }

    bool isLoaded() const noexcept {
        return m_isLoaded;
    }

 private:
    bool loadCsv(const std::string& aPath) {
        std::ifstream file(aPath);
        if (!file.is_open()) {
            return false;
        }

        std::vector<Entry> temp;
        std::string line;

        while (std::getline(file, line)) {
            try {
                temp.emplace_back(parseLine(line));
            } catch (const std::exception& e) {
                return false;
            }
        }

        std::unique_lock lock(m_mutex);
        m_data = std::move(temp);

        return true;
    }

    Entry parseLine(const std::string& aLine) const {
        std::istringstream ss(aLine);
        std::string token;

        // Get label
        if (!std::getline(ss, token, kMnistCsvDelimiter)) {
            throw std::runtime_error("Missing label in CSV file");
        }

        int value = std::stoi(token);
        if (value < 0 || value > 9) {
            throw std::runtime_error("Invalid label value: " +
                                     std::to_string(value));
        }
        Label label = static_cast<Label>(value);

        // Get image
        Image image{};
        std::size_t pixelCount = 0;
        while (std::getline(ss, token, kMnistCsvDelimiter)) {
            if (pixelCount >= kMnistImageSize) {
                throw std::runtime_error("Too many pixel values in line");
            }

            value = std::stoi(token);
            if (value < 0 || value > 255) {
                throw std::runtime_error("Pixel out of range: " +
                                         std::to_string(value));
            }

            image[pixelCount++] = static_cast<Pixel>(value);
        }

        if (pixelCount != kMnistImageSize) {
            throw std::runtime_error("Invalid pixel count: expected " +
                    std::to_string(kMnistImageSize) + ", got " +
                    std::to_string(pixelCount));
        }

        return {label, image};
    }

    container_type m_data;
    mutable std::shared_mutex m_mutex;
    bool m_isLoaded = false;
};

#endif  // LIB_INCLUDE_MNISTCSVDATASET_HPP_

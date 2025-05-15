// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "include/mnistcsvdataset.hpp"

class MnistCsvDataSetFixture : public ::testing::Test {
 protected:
    void TearDown() override {
        if (!m_path.empty()) {
            removeTemporaryCsv(m_path);
            m_path.clear();
        }
    }

    void createDatasetWithLine(const std::string& line) {
        m_path = createTemporaryCsv(line);
    }

    MnistCsvDataSet createDataset() const {
        return MnistCsvDataSet(m_path);
    }

    static std::string generateValidCsv(char label, char value) {
        return generateCsvLine(
            label, value,
            MnistCsvDataSet::kMnistCsvDelimiter,
            MnistCsvDataSet::kMnistImageSize);
    }

    static void removeTemporaryCsv(const std::string& aFileName) {
        std::remove(aFileName.c_str());
    }

    static std::string createTemporaryCsv(const std::string& aContent,
                    const std::string& aFileName = "temp_mnist.csv") {
        std::ofstream out(aFileName);
        out << aContent;
        out.close();
        return aFileName;
    }

    static std::string generateCsvLine(char aLabel, char aValue,
                                char aDelimiter, std::size_t aCount) {
        std::ostringstream oss;
        oss << aLabel << aDelimiter;
        for (size_t i = 0; i < aCount; ++i) {
            if (i != 0) oss << aDelimiter;
            oss << aValue;
        }
        return oss.str();
    }

 private:
    std::string m_path;
};


TEST_F(MnistCsvDataSetFixture, ValidCsv_IsOpen) {
    createDatasetWithLine(generateValidCsv('5', '0'));
    auto dataset = createDataset();

    ASSERT_TRUE(dataset.is_open());
}

TEST_F(MnistCsvDataSetFixture, ValidCsv_Size) {
    constexpr size_t expectedValue = 1;

    createDatasetWithLine(generateValidCsv('5', '0'));
    auto dataset = createDataset();

    EXPECT_EQ(dataset.size(), expectedValue);
}

TEST_F(MnistCsvDataSetFixture, ValidCsv_Label) {
    constexpr uint8_t expectedValue = 5;

    createDatasetWithLine(generateValidCsv('5', '0'));
    auto dataset = createDataset();

    ASSERT_EQ(dataset[0].first, expectedValue);
}

TEST_F(MnistCsvDataSetFixture, ValidCsv_Image) {
    createDatasetWithLine(generateValidCsv('5', '0'));
    auto dataset = createDataset();
    const auto& image = dataset[0].second;

    for (const auto pixel : image) {
        EXPECT_EQ(pixel, 0);
    }
}

TEST_F(MnistCsvDataSetFixture, ValidCsv_At_Label) {
    constexpr uint8_t expectedValue = 5;

    createDatasetWithLine(generateValidCsv('5', '0'));
    auto dataset = createDataset();

    EXPECT_EQ(dataset.at(0).first, expectedValue);
}

TEST_F(MnistCsvDataSetFixture, ValidCsv_At_Image) {
    createDatasetWithLine(generateValidCsv('5', '0'));
    auto dataset = createDataset();
    const auto& image = dataset.at(0).second;

    for (const auto pixel : image) {
        EXPECT_EQ(pixel, 0);
    }
}

TEST_F(MnistCsvDataSetFixture, InvalidCsv_TooFewPixels) {
    const std::string line = generateCsvLine(
        '5', '0', MnistCsvDataSet::kMnistCsvDelimiter,
        MnistCsvDataSet::kMnistImageSize - 1);
    createDatasetWithLine(line);
    auto dataset = createDataset();

    EXPECT_FALSE(dataset.is_open());
}

TEST_F(MnistCsvDataSetFixture, InvalidCsv_TooManyPixels) {
    const std::string line = generateCsvLine(
        '5', '0', MnistCsvDataSet::kMnistCsvDelimiter,
        MnistCsvDataSet::kMnistImageSize + 1);
    createDatasetWithLine(line);
    auto dataset = createDataset();

    EXPECT_FALSE(dataset.is_open());
}

TEST_F(MnistCsvDataSetFixture, InvalidCsv_InvalidLabel) {
    const std::string line = generateValidCsv('a', '0');  // 'a' = 97
    createDatasetWithLine(line);
    auto dataset = createDataset();

    EXPECT_FALSE(dataset.is_open());
}

TEST_F(MnistCsvDataSetFixture, InvalidCsv_InvalidDelimiter) {
    const std::string line = generateCsvLine(
        '5', '0', '.', MnistCsvDataSet::kMnistImageSize);  // '.' instead ','
    createDatasetWithLine(line);
    auto dataset = createDataset();

    EXPECT_FALSE(dataset.is_open());
}

TEST_F(MnistCsvDataSetFixture, InvalidCsv_InvalidPixel) {
    std::ostringstream oss;
    oss << '3' << MnistCsvDataSet::kMnistCsvDelimiter;
    for (size_t i = 0; i < MnistCsvDataSet::kMnistImageSize; ++i) {
        if (i != 0) oss << MnistCsvDataSet::kMnistCsvDelimiter;
        oss << ((i == 10) ? 300 : 0);  // Invalid pixel value
    }
    createDatasetWithLine(oss.str());
    auto dataset = createDataset();

    EXPECT_FALSE(dataset.is_open());
}

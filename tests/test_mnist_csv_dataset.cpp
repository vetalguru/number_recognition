// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

#include "include/mnistcsvdataset.hpp"

class MnistCsvDataSetFixtureBase : public ::testing::Test {
 public:
    static constexpr MnistCsvDataSet::Label_t kTestLabelValue = 5;
    static constexpr MnistCsvDataSet::Pixel_t kTestPixelValue = 150;
    static constexpr char kDefaultDelimiter =
        MnistCsvDataSet::kMnistCsvDelimiter;
    static constexpr std::size_t kImageSize =
        MnistCsvDataSet::kMnistImageSize;
    static constexpr char kTestFileName[] = "temp_mnist_dataset.csv";

    static MnistCsvDataSet getDataset() {
        return MnistCsvDataSet(csvPath);
    }

    static inline std::string csvPath;

 protected:
    static void TearDownTestSuite() {
        if (!csvPath.empty()) {
            std::remove(csvPath.c_str());
        }
    }

    static std::string generateCsvLine(
        MnistCsvDataSet::Label_t aLabel,
        MnistCsvDataSet::Pixel_t aPixel,
        char aDelimiter,
        std::size_t aCount) {
        std::ostringstream oss;
        oss << static_cast<int>(aLabel) << aDelimiter;
        for (std::size_t i = 0; i < aCount; ++i) {
            if (i != 0) {
                oss << aDelimiter;
            }

            oss << static_cast<int>(aPixel);
        }
        return oss.str();
    }

    static std::string createTempCsvFile(
        const std::vector<std::string>& aLines) {
        std::ofstream out(kTestFileName);
        for (const auto& line : aLines) {
            out << line << "\n";
        }
        out.close();
        return std::string(kTestFileName);
    }
};

// Fixture for valid dataset
class MnistCsvDataSetValidFixture : public MnistCsvDataSetFixtureBase {
 protected:
    static void SetUpTestSuite() {
        csvPath = createTempCsvFile({
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kDefaultDelimiter, kImageSize),
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kDefaultDelimiter, kImageSize),
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kDefaultDelimiter, kImageSize),
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kDefaultDelimiter, kImageSize),
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kDefaultDelimiter, kImageSize)
        });
    }
};

// Fixture for invalid pixel value (e.g., 300)
class MnistCsvDataSetInvalidPixelFixture :
                        public MnistCsvDataSetFixtureBase {
 protected:
    static void SetUpTestSuite() {
        std::ostringstream oss;
        oss << static_cast<int>(kTestLabelValue) << kDefaultDelimiter;
        for (size_t i = 0; i < kImageSize; ++i) {
            if (i != 0) {
                oss << kDefaultDelimiter;
            }

            // Insert incorrect value into position 10
            oss << ((i == 10) ? 300 : 0);
        }
        csvPath = createTempCsvFile({oss.str()});
    }
};

// Fixture for invalid delimiter
class MnistCsvDataSetInvalidDelimiterFixture :
                        public MnistCsvDataSetFixtureBase {
 protected:
    static constexpr char kWrongDelimiter = '@';

    static void SetUpTestSuite() {
        csvPath = createTempCsvFile({
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kWrongDelimiter, kImageSize)
        });
    }
};

// Fixture for too few pixels
class MnistCsvDataSetTooFewPixelsFixture : public MnistCsvDataSetFixtureBase {
 protected:
    static constexpr std::size_t kWrongImageSize = kImageSize - 1;

    static void SetUpTestSuite() {
        csvPath = createTempCsvFile({
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kDefaultDelimiter, kWrongImageSize)
        });
    }
};

// Fixture for too many pixels
class MnistCsvDataSetTooManyPixelsFixture : public MnistCsvDataSetFixtureBase {
 protected:
    static constexpr std::size_t kWrongImageSize = kImageSize + 1;

    static void SetUpTestSuite() {
        csvPath = createTempCsvFile({
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kDefaultDelimiter, kWrongImageSize)
        });
    }
};

// Fixture for wrong label > 9
class MnistCsvDataSetWrongLabelFixture : public MnistCsvDataSetFixtureBase {
 protected:
        static constexpr MnistCsvDataSet::Label_t kTestLabelValue = 10;

    static void SetUpTestSuite() {
        csvPath = createTempCsvFile({
            generateCsvLine(kTestLabelValue, kTestPixelValue,
                            kDefaultDelimiter, kImageSize)
        });
    }
};

//=================================================================================

TEST_F(MnistCsvDataSetValidFixture, CopyConstructor_CheckIsOpen) {
    const auto original = getDataset();

    EXPECT_NO_THROW({
        const auto copy(original);
        EXPECT_TRUE(original.isLoaded());
        EXPECT_TRUE(copy.isLoaded());
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyConstructor_CheckSize) {
    constexpr std::size_t expectedSize = 5;  // 5 lines in the test file
    const auto original = getDataset();

    EXPECT_NO_THROW({
        const auto copy(original);
        EXPECT_EQ(copy.size(), expectedSize);
        EXPECT_EQ(original.size(), expectedSize);
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyConstructor_CheckLabel) {
    const auto original = getDataset();

    EXPECT_NO_THROW({
        const auto copy(original);
        EXPECT_EQ(copy.at(0).first, kTestLabelValue);
        EXPECT_EQ(original.at(0).first, kTestLabelValue);
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyConstructor_CheckImageSize) {
    auto original = getDataset();

    EXPECT_NO_THROW({
        auto copy(original);
        EXPECT_EQ(copy.at(0).second.size(), MnistCsvDataSet::kMnistImageSize);
        EXPECT_EQ(original.at(0).second.size(),
                  MnistCsvDataSet::kMnistImageSize);
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyConstructor_CheckImagePixels) {
    auto original = getDataset();

    EXPECT_NO_THROW({
        auto copy(original);
        for (size_t i = 0; i < copy.at(0).second.size(); ++i) {
            EXPECT_EQ(copy.at(0).second.at(i), kTestPixelValue);
            EXPECT_EQ(original.at(0).second.at(i), kTestPixelValue);
        }
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyOperator_IsOpen) {
    auto original = getDataset();

    EXPECT_NO_THROW({
        auto copy = original;
        EXPECT_TRUE(original.isLoaded());
        EXPECT_TRUE(copy.isLoaded());
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyOperator_Size) {
    constexpr std::size_t expectedSize = 5;  // 5 lines in test file
    auto original = getDataset();

    EXPECT_NO_THROW({
        auto copy = original;
        EXPECT_EQ(copy.size(), expectedSize);
        EXPECT_EQ(original.size(), expectedSize);
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyOperator_CheckLabel) {
    auto original = getDataset();

    EXPECT_NO_THROW({
        auto copy = original;
        EXPECT_EQ(copy.at(0).first, kTestLabelValue);
        EXPECT_EQ(original.at(0).first, kTestLabelValue);
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyOperator_CheckImageSize) {
    auto original = getDataset();

    EXPECT_NO_THROW({
        auto copy = original;
        EXPECT_EQ(copy.at(0).second.size(), MnistCsvDataSet::kMnistImageSize);
        EXPECT_EQ(original.at(0).second.size(),
                  MnistCsvDataSet::kMnistImageSize);
    });
}

TEST_F(MnistCsvDataSetValidFixture, CopyOperator_CheckImagePixels) {
    const auto original = getDataset();

    EXPECT_NO_THROW({
        const auto copy = original;
        for (size_t i = 0; i < copy.at(0).second.size(); ++i) {
            EXPECT_EQ(copy.at(0).second.at(i), kTestPixelValue);
            EXPECT_EQ(original.at(0).second.at(i), kTestPixelValue);
        }
    });
}

TEST_F(MnistCsvDataSetValidFixture, MoveConstructor_CheckIdOpen) {
    auto original = getDataset();

    const auto move(std::move(original));

    EXPECT_TRUE(move.isLoaded());
}

TEST_F(MnistCsvDataSetValidFixture, MoveConstructor_CheckSize) {
    constexpr size_t expectedSize = 5;  // 5 lines in test file

    auto original = getDataset();
    const auto move(std::move(original));

    ASSERT_EQ(move.size(), expectedSize);
}

TEST_F(MnistCsvDataSetValidFixture, MoveConstructor_CheckLabel) {
    auto original = getDataset();

    const auto move(std::move(original));

    EXPECT_NO_THROW({
        ASSERT_EQ(move.at(0).first, kTestLabelValue);
    });
}

TEST_F(MnistCsvDataSetValidFixture, MoveConstructor_ImageSize) {
    auto original = getDataset();

    const auto move(std::move(original));

    EXPECT_NO_THROW(
        EXPECT_EQ(move.at(0).second.size(), MnistCsvDataSet::kMnistImageSize););
}

TEST_F(MnistCsvDataSetValidFixture, MoveConstructor_ImagePixels) {
    auto original = getDataset();
    const auto move(std::move(original));

    EXPECT_NO_THROW({
        for (size_t i = 0; i < move.at(0).second.size(); ++i) {
            EXPECT_EQ(move.at(0).second.at(0), kTestPixelValue);
        }
    });
}

TEST_F(MnistCsvDataSetValidFixture, ValidCsv_IsOpen) {
    const auto dataset = getDataset();

    ASSERT_TRUE(dataset.isLoaded());
}

TEST_F(MnistCsvDataSetValidFixture, ValidCsv_Size) {
    constexpr size_t expectedValue = 5;  // 5 lines in test file
    const auto dataset = getDataset();

    EXPECT_EQ(dataset.size(), expectedValue);
}

TEST_F(MnistCsvDataSetValidFixture, ValidCsv_Label) {
    const auto dataset = getDataset();

    ASSERT_EQ(dataset.at(0).first, kTestLabelValue);
}

TEST_F(MnistCsvDataSetValidFixture, ValidCsv_Image) {
    const auto dataset = getDataset();
    const auto& image = dataset.at(0).second;

    for (const auto pixel : image) {
        EXPECT_EQ(pixel, kTestPixelValue);
    }
}

TEST_F(MnistCsvDataSetValidFixture, ValidCsv_At_Label) {
    const auto dataset = getDataset();

    EXPECT_EQ(dataset.at(0).first, kTestLabelValue);
}

TEST_F(MnistCsvDataSetValidFixture, ValidCsv_At_Image) {
    const auto dataset = getDataset();
    const auto& image = dataset.at(0).second;

    for (const auto pixel : image) {
        EXPECT_EQ(pixel, kTestPixelValue);
    }
}

TEST_F(MnistCsvDataSetInvalidPixelFixture,
       InvalidCsv_InvalidPixelValue_ShouldFail) {
    const auto dataset = getDataset();

    ASSERT_FALSE(dataset.isLoaded());
}

TEST_F(MnistCsvDataSetInvalidDelimiterFixture,
       InvalidCsv_InvalidDelimiter_ShouldFail) {
    const auto dataset = getDataset();

    ASSERT_FALSE(dataset.isLoaded());
}

TEST_F(MnistCsvDataSetTooFewPixelsFixture,
       InvalidCsv_TooFewPixels_ShouldFail) {
    const auto dataset = getDataset();

    ASSERT_FALSE(dataset.isLoaded());
}

TEST_F(MnistCsvDataSetTooManyPixelsFixture,
       InvalidCsv_TooManyPixels_ShouldFail) {
    const auto dataset = getDataset();

    ASSERT_FALSE(dataset.isLoaded());
}

TEST_F(MnistCsvDataSetWrongLabelFixture,
       InvalidCsv_WrongLabelValue_ShouldFail) {
    const auto dataset  = getDataset();

    ASSERT_FALSE(dataset.isLoaded());
}

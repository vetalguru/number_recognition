// Copyright 2024 Vitalii Shkibtan

#include <gtest/gtest.h>

#include "include/helloworld.h"

constexpr int kTestNumber = 42;

TEST(GetTestNumber, GetTestNumber) {
    EXPECT_EQ(kTestNumber, GetTestNumber());
}

TEST(GetTestNumber, GetTestNumberLogInfoStream) {
    std::string output;
    int testNumber;

    testing::internal::CaptureStdout();
    testNumber = GetTestNumber();
    output = testing::internal::GetCapturedStdout();

    EXPECT_NE(std::string::npos, output.find("INFO"));
    EXPECT_NE(std::string::npos, output.find("GetTestNumber() called with result: "));
    EXPECT_NE(std::string::npos, output.find(std::to_string(kTestNumber)));
    EXPECT_EQ(kTestNumber, testNumber);
}

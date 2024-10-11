// Copyright 2024 Vitalii Shkibtan

#include <gtest/gtest.h>

#include "../../lib/include/helloworld.h"

TEST(GetTestNumber, SimpleTest) {
    EXPECT_EQ(42, GetTestNumber());
}


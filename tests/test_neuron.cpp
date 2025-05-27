// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include <gtest/gtest.h>

#include <climits>

#include <vector>

#include "include/neuron.hpp"


TEST(NeuronTest, Inititalization) {
    constexpr int inputs = 3;
    constexpr double expected = 0.0;

    Neuron neuron(inputs, Neuron::ActivationFunction::SIGMOID);

    EXPECT_EQ(neuron.cweights().size(), inputs);
    for (size_t i = 0; i < inputs; ++i) {
        EXPECT_DOUBLE_EQ(neuron.weight(i), expected);
    }
    EXPECT_DOUBLE_EQ(neuron.bias(), expected);
}

TEST(NeuronTest, SetBias) {
    constexpr int inputs = 2;
    constexpr double expected = 42.0;

    Neuron neuron(inputs, Neuron::ActivationFunction::SIGMOID);
    neuron.setBias(expected);

    EXPECT_DOUBLE_EQ(neuron.bias(), expected);
}

TEST(NeuronTest, SetWeights) {
    constexpr int input = 3;
    constexpr double expected = 42.0;

    Neuron neuron(input, Neuron::ActivationFunction::SIGMOID);
    // Set weights
    for (size_t i = 0; i < input; ++i) {
        neuron.setWeight(i, expected);
    }

    // Check weights
    for (size_t i = 0; i < input; ++i) {
        EXPECT_DOUBLE_EQ(neuron.weight(i), expected);
    }
}

TEST(NeuronTest, SetWeight_OutOfRange) {
    constexpr int input = 3;
    constexpr double expected = 0.0;
    constexpr double outOfRangeValue = 42.0;

    Neuron neuron(input, Neuron::ActivationFunction::SIGMOID);
    neuron.setWeight(static_cast<size_t>(INT_MAX), outOfRangeValue);

    EXPECT_DOUBLE_EQ(neuron.weight(static_cast<size_t>(INT_MAX)), expected);
}

TEST(NeuronTest, SetWeight) {
    constexpr int inputs = 3;
    Neuron neuron(inputs, Neuron::ActivationFunction::SIGMOID);

    // Set weights
    for (size_t i = 0; i < inputs; ++i) {
        neuron.setWeight(i, static_cast<double>(i));
    }

    // Check method weight()
    for (size_t i = 0; i < inputs; ++i) {
        EXPECT_DOUBLE_EQ(static_cast<double>(i), neuron.weight(i));
    }

    // Check method weights()
    std::vector<double>& weights = neuron.weights();
    EXPECT_EQ(inputs, weights.size());
    for (size_t i = 0; i < inputs; ++i) {
        EXPECT_DOUBLE_EQ(static_cast<double>(i), weights[i]);
    }

    // Check method const weights()
    const std::vector<double>& cweights = neuron.cweights();
    EXPECT_EQ(inputs, cweights.size());
    for (size_t i = 0; i < inputs; ++i) {
        EXPECT_DOUBLE_EQ(static_cast<double>(i), cweights[i]);
    }
}

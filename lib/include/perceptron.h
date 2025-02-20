// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef LIB_INCLUDE_PERCEPTRON_H_
#define LIB_INCLUDE_PERCEPTRON_H_

#include <vector>

#include "./neuron.h"

class NeuroNetwork {
 public:
    explicit NeuroNetwork(const std::vector<int> aLayers);

    // NOLINTNEXTLINE(build/include_what_you_use)
    std::vector<std::vector<double>> forward(
        const std::vector<double>& aInput) const;

    void train(const std::vector<std::vector<double>>& aInputData,
                const std::vector<std::vector<double>>& aTargetData,
                int epochs, double aLearningRate);

 private:
    std::vector<std::vector<Neuron>> m_layers;
};

#endif  // LIB_INCLUDE_PERCEPTRON_H_

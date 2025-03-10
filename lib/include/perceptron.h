// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef LIB_INCLUDE_PERCEPTRON_H_
#define LIB_INCLUDE_PERCEPTRON_H_

#include <vector>

#include "include/neuron.h"

class Perceptron {
 public:
    Perceptron() = default;
    Perceptron(const std::vector<int>& aLayers,
        Neuron::ActivationFunction aFunction =
            Neuron::ActivationFunction::SIGMOID);

    bool initializeNetwork(const std::vector<int>& aLayers,
        Neuron::ActivationFunction aFunction =
            Neuron::ActivationFunction::SIGMOID);
    bool isConfigured() const;

    // NOLINTNEXTLINE(build/include_what_you_use)
    std::vector<std::vector<double>> forward(
        const std::vector<double>& aInput) const;

    void train(const std::vector<std::vector<double>>& aInputData,
                const std::vector<std::vector<double>>& aTargetData,
                int epochs, double aLearningRate);

    bool isTrained() const;

    const std::vector<std::vector<Neuron>>& layers() const;

    bool setNeuronWeights(size_t aLayerIndex, size_t aNeuronIndex,
        const std::vector<double>& aWeights);

    bool setNeuronBias(size_t aLayerIndex, size_t aNeuronIndex,
        double aBias);

 private:
    std::vector<std::vector<Neuron>> m_layers;
    bool m_isConfigured = false;
    bool m_isTrained = false;
};

#endif  // LIB_INCLUDE_PERCEPTRON_H_

// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "include/perceptron.h"

#include <complex>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>


NeuroNetwork::NeuroNetwork(const std::vector<int> aLayers) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    m_layers.resize(aLayers.size() - 1);
    for (size_t i = 1; i < aLayers.size(); ++i) {
        m_layers[i - 1].resize(aLayers[i], Neuron(aLayers[i - 1]));
        for (auto& neuron : m_layers[i - 1]) {
            for (auto& weight : neuron.weights()) {
                weight = dist(gen);
            }
            neuron.setBias(dist(gen));
        }
    }
}

std::vector<std::vector<double>> NeuroNetwork::forward(
        const std::vector<double>& aInput) const {
    std::vector<std::vector<double>> activations;
    activations.push_back(aInput);  // Push input layer

    for (auto& layer : m_layers) {
        std::vector<double> newActivations;
        for (auto& neuron : layer) {
            newActivations.push_back(neuron.output(activations.back()));
        }
        activations.push_back(newActivations);  // Add another layer
    }

    return activations;
}

void NeuroNetwork::train(const std::vector<std::vector<double>>& aInputData,
            const std::vector<std::vector<double>>& aTargetData,
            int epochs, double aLearningRate) {
    // For all epochs
    for (int epoch = 0; epoch < epochs; ++epoch) {
        double totalError = 0.0;

        // For all samples
        for (size_t sample = 0; sample < aInputData.size(); ++sample) {
            // 1 Stage: Forward pass
            std::vector<std::vector<double>> activations =
                // NOLINTNEXTLINE(build/include_what_you_use)
                forward(aInputData[sample]);

            // 2 Stage: Backpropagation(calculate errors and gradients)
            std::vector<std::vector<double>> deltas(m_layers.size());
            for (int i = static_cast<int>(m_layers.size()) - 1; i >= 0; --i) {
                deltas[i].resize(m_layers[i].size());

                for (size_t j = 0; j < m_layers[i].size(); ++j) {
                    if (i == m_layers.size() - 1) {
                        // output layer
                        double error = aTargetData[sample][j] -
                            activations[i + 1][j];
                        deltas[i][j] = error *
                            m_layers[i][j].activateDerivative(
                                activations[i + 1][j]);
                        totalError += error * error;
                    } else {
                        // hidden layers
                        double error = 0.0;
                        for (size_t k = 0; k < m_layers[i + 1].size(); ++k) {
                            const auto& weights = m_layers[i + 1][k].weights();
                            if (j < weights.size()) {
                                error += deltas[i + 1][k] * weights[j];
                            }
                        }
                        deltas[i][j] = error *
                            m_layers[i][j].activateDerivative(
                                activations[i + 1][j]);
                    }
                }
            }

            // 3 Stage: Update weights
            for (size_t i = 0; i < m_layers.size(); ++i) {
                for (size_t j = 0; j < m_layers[i].size(); ++j) {
                    m_layers[i][j].updateWeights(activations[i],
                        aLearningRate, deltas[i][j]);
                }
            }
        }

        std::cout << "Epoch " << epoch + 1
            << ", Error: " << totalError / aInputData.size() << std::endl;
    }
}

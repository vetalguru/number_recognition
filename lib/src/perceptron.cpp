// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "include/perceptron.h"

#include <complex>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#include "include/logger.hpp"


Perceptron::Perceptron(const std::vector<int>& aLayers,
    Neuron::ActivationFunction aFunction) {
    initializeNetwork(aLayers, aFunction);
}

bool Perceptron::initializeNetwork(const std::vector<int>& aLayers,
    Neuron::ActivationFunction aFunction) {
    m_isConfigured = false;
    m_isTrained = false;
    m_layers.clear();

    if (aLayers.size() < 2) {
        LOG_ERROR << "Network must have at least input and output layers";
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    m_layers.resize(aLayers.size() - 1);  // Without first (input) layer
    for (size_t layerIndex = 1; layerIndex < aLayers.size(); ++layerIndex) {
        m_layers[layerIndex - 1].resize(aLayers[layerIndex],
            Neuron(aLayers[layerIndex - 1], aFunction));

        for (auto& neuron : m_layers[layerIndex - 1]) {
            for (size_t neuronIndex = 0; neuronIndex < neuron.weights().size();
                    ++neuronIndex) {
                neuron.setWeight(neuronIndex, dist(gen));
            }

            neuron.setBias(dist(gen));
        }
    }

    LOG_INFO << "Network configured: ";
    LOG_INFO << "Layer " << 0 << ": " << aLayers[0] << " neurons";
    for (size_t i = 0; i < m_layers.size(); ++i) {
        LOG_INFO << "Layer " << i + 1 << ": " << m_layers[i].size()
            << " neurons";
    }

    m_isConfigured = true;
    return true;
}

bool Perceptron::isConfigured() const {
    return m_isConfigured;
}

std::vector<std::vector<double>> Perceptron::forward(
        const std::vector<double>& aInput) const {
    std::vector<std::vector<double>> activations;
    activations.push_back(aInput);  // Push input layer

    for (const auto& layer : m_layers) {
        std::vector<double> newActivations;
        for (auto& neuron : layer) {
            newActivations.push_back(neuron.output(activations.back()));
        }
        activations.push_back(newActivations);  // Add another layer
    }

    return activations;
}

void Perceptron::train(const std::vector<std::vector<double>>& aInputData,
            const std::vector<std::vector<double>>& aTargetData,
            int epochs, double aLearningRate) {
    m_isTrained = false;

    if (!m_isConfigured) {
        LOG_ERROR << "Network is not configured successfully";
        return;
    }

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

        LOG_INFO << "Epoch " << epoch + 1
            << ", Error: " << totalError / aInputData.size();
    }

    m_isTrained = true;
}

bool Perceptron::isTrained() const {
    return m_isTrained;
}

const std::vector<std::vector<Neuron>>& Perceptron::layers() const {
    return m_layers;
}

bool Perceptron::setNeuronWeights(size_t aLayerIndex, size_t aNeuronIndex,
    const std::vector<double>& aWeights) {
    if (aLayerIndex >= m_layers.size()) {
        LOG_ERROR << "Layer index is out of indexes range";
        return false;
    }

    if (aNeuronIndex >= m_layers[aLayerIndex].size()) {
        LOG_ERROR << "Invalid neuron index for layer " << aLayerIndex + 1;
        return false;
    }

    if (aWeights.size() !=
            m_layers[aLayerIndex][aNeuronIndex].weights().size()) {
        LOG_ERROR << "The size of the weights vector does not "
            << "match the number of weights in the neuron";
        return false;
    }

    for (size_t i = 0; i < aWeights.size(); ++i) {
        m_layers[aLayerIndex][aNeuronIndex].setWeight(i, aWeights[i]);
    }

    return true;
}

bool Perceptron::setNeuronBias(size_t aLayerIndex, size_t aNeuronIndex,
    double aBias) {
    if (aLayerIndex >= m_layers.size()) {
        LOG_ERROR << "Layer index is out of indexes range";
        return false;
    }

    if (aNeuronIndex >= m_layers[aLayerIndex].size()) {
        LOG_ERROR << "Invalid neuron index for layer " << aLayerIndex + 1;
        return false;
    }

    m_layers[aLayerIndex][aNeuronIndex].setBias(aBias);
    return true;
}

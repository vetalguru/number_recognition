// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "include/neuron.h"

#include <complex>
#include <stdexcept>
#include <vector>


Neuron::Neuron(int aInputs)
    : m_weights(aInputs, 0.0)
    , m_bias(0.0) {
}

double Neuron::bias() const noexcept {
    return m_bias;
}

void Neuron::setBias(double aBias) noexcept {
    m_bias = aBias;
}

double Neuron::weight(size_t aIndex) const noexcept {
    if (aIndex >= m_weights.size()) {
        return 0.0;
    }

    return m_weights[aIndex];
}

void Neuron::setWeight(size_t aIndex, double aValue) noexcept {
    if (aIndex >= m_weights.size()) {
        return;
    }

    m_weights[aIndex] = aValue;
}

std::vector<double>& Neuron::weights() noexcept {
    return m_weights;
}

const std::vector<double>& Neuron::weights() const noexcept {
    return m_weights;
}

double Neuron::output(const std::vector<double>& aInput) const noexcept {
    if (aInput.size() != m_weights.size()) {
        return 0.0;
    }
    return activate(sum(aInput));
}

void Neuron::updateWeights(const std::vector<double>& aInputs,
    double aLearningRate, double delta) noexcept {
    for (size_t i = 0; i < m_weights.size(); ++i) {
        setWeight(i, weight(i) + aLearningRate * delta * (aInputs[i]));
    }

    m_bias += aLearningRate * delta;
}

double Neuron::activate(double aValue) const noexcept {
    return sigmoid(aValue);
}

double Neuron::sigmoid(double aValue) const noexcept {
    return 1.0 / (1.0 + std::exp(-aValue));
}

double Neuron::activateDerivative(double aValue) const noexcept {
    return sigmoidDerivative(aValue);
}

double Neuron::sigmoidDerivative(double aValue) const noexcept {
    const double sig = sigmoid(aValue);
    return sig * (1 - sig);
}

double Neuron::sum(const std::vector<double>& aInputs) const noexcept {
    double result = m_bias;
    if (aInputs.size() != m_weights.size()) {
        return 0.0;
    }

    for (size_t i = 0; i < aInputs.size(); ++i) {
        result += aInputs[i] * m_weights[i];
    }

    return result;
}

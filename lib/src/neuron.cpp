// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "neuron.h"

#include <complex>
#include <stdexcept>

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

double Neuron::weight(int aIndex) const {
    return m_weights[aIndex];
}

void Neuron::setWeight(int aIndex, double aValue) {
    m_weights[aIndex] = aValue;
}

std::vector<double>& Neuron::weights() {
    return m_weights;
}

double Neuron::output(const std::vector<double>& aInput) const {
    return activate(sum(aInput));
}

void Neuron::updateWeights(const std::vector<double>& aInputs, double aLearningRate, double delta) {
    for (size_t i = 0; i < m_weights.size(); ++i) {
        setWeight(i, weight(i) + aLearningRate * delta * (aInputs[i]));
    }

    m_bias += aLearningRate * delta;
}

double Neuron::activate(double aValue) const {
    return sigmoid(aValue);
}

double Neuron::sigmoid(double aValue) const {
    return 1.0 / (1.0 + std::exp(-aValue));
}

double Neuron::activateDerivative(double aValue) const {
    return sigmoidDerivative(aValue);
}

double Neuron::sigmoidDerivative(double aValue) const {
    const double sig = sigmoid(aValue);
    return sig * (1 - sig);
}

double Neuron::sum(const std::vector<double>& aInputs) const {
    double result = m_bias;
    for (size_t i = 0; i < aInputs.size(); ++i) {
        result += aInputs[i] * m_weights[i];
    }

    return result;
}

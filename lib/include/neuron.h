// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef LIB_INCLUDE_NEURON_H_
#define LIB_INCLUDE_NEURON_H_

#include <vector>

class Neuron {
 public:
    enum class ActivationFunction {
        SIGMOID,
        RELU
    };

 public:
    Neuron(int aNumInputs, ActivationFunction aFunction);

    double bias() const noexcept;
    void setBias(double aBias) noexcept;

    double weight(size_t aIndex) const noexcept;
    void setWeight(size_t aIndex, double aValue) noexcept;
    std::vector<double>& weights() noexcept;
    const std::vector<double>& weights() const noexcept;

    double output(const std::vector<double>& aInput) const noexcept;
    void updateWeights(const std::vector<double>& aInputs,
        double aLearningRate, double delta) noexcept;

    double activate(double aValue) const noexcept;
    double activateDerivative(double aValue) const noexcept;

 private:
    double sigmoid(double aValue) const noexcept;
    double sigmoidDerivative(double aValue) const noexcept;

    double relu(double aValue) const noexcept;
    double reluDerivative(double aValue) const noexcept;

    double sum(const std::vector<double>& aInputs) const noexcept;

 private:
    std::vector<double> m_weights;
    double m_bias;
    ActivationFunction m_function;
};

#endif  // LIB_INCLUDE_NEURON_H_

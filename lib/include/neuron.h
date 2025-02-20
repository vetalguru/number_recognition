// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef LIB_INCLUDE_NEURON_H_
#define LIB_INCLUDE_NEURON_H_

#include <vector>

class Neuron {
 public:
    explicit Neuron(int aNumInputs);

    double bias() const noexcept;
    void setBias(double aBias) noexcept;

    double weight(int aIndex) const;
    void setWeight(int aIndex, double aValue);
    std::vector<double>& weights();

    double output(const std::vector<double>& aInput) const;
    void updateWeights(const std::vector<double>& aInputs,
        double aLearningRate, double delta);

    double activate(double aValue) const;
    double activateDerivative(double aValue) const;

 private:
    double sigmoid(double aValue) const;
    double sigmoidDerivative(double aValue) const;

    double sum(const std::vector<double>& aInputs) const;

 private:
    std::vector<double> m_weights;
    double m_bias;
};

#endif  // LIB_INCLUDE_NEURON_H_

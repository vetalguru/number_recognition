// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "include/mnistlearningform.hpp"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>
#include <QMetaObject>
#include <QMessageBox>

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

#include <boost/json.hpp>

#include "include/perceptron.hpp"
#include "include/logger.hpp"
#include <include/mnistcsvdataset.hpp>

// Unnamed namespace to restrict the scope of constants to this translation unit
namespace {
constexpr char kDelimeter = ',';

constexpr int kNumClasses = 10;      // Numbers from 0 to 9
constexpr int kImageSize = 28 * 28;  // Images 28 px x 28 px

static inline std::vector<double> ToOneHot(uint8_t aLabel,
            size_t aNumClasses = 10) {
    std::vector<double> vec(aNumClasses, 0.0);
    vec[aLabel] = 1.0;
    return vec;
}

static inline std::vector<double> NormalizeImage(
    const MnistCsvDataSet::Image_t& image) {
    std::vector<double> result(image.size());

    std::transform(image.begin(), image.end(), result.begin(),
        [](uint8_t px) { return static_cast<double>(px) / 255.0; });

    return result;
}
}  // namespace

MnistLearningForm::MnistLearningForm(QWidget *parent)
    : QDialog(parent) {
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create input group box
    QGridLayout *inputFilesLayout = new QGridLayout(this);

    // Train file block
    QLabel *trainFileLabel = new QLabel("Train file: ", this);
    m_trainFileEdit = new QLineEdit(this);
    QPushButton *trainFileButton = new QPushButton("File...", this);
    inputFilesLayout->addWidget(trainFileLabel, 0, 0, Qt::AlignRight);
    inputFilesLayout->addWidget(m_trainFileEdit, 0, 1, Qt::AlignCenter);
    inputFilesLayout->addWidget(trainFileButton, 0, 2, Qt::AlignLeft);

    // Save model block
    QLabel *outputFileLabel = new QLabel("Output file: ", this);
    m_outputFileEdit = new QLineEdit(this);
    QPushButton *outputFileButton = new QPushButton("File...", this);
    inputFilesLayout->addWidget(outputFileLabel, 2, 0, Qt::AlignRight);
    inputFilesLayout->addWidget(m_outputFileEdit, 2, 1, Qt::AlignCenter);
    inputFilesLayout->addWidget(outputFileButton, 2, 2, Qt::AlignLeft);

    QGroupBox *inputBox = new QGroupBox(this);
    inputBox->setLayout(inputFilesLayout);

    // Buttons layout
    QHBoxLayout *buttonsLayout = new QHBoxLayout(this);

    m_trainButton = new QPushButton("Train model", this);
    m_trainButton->setEnabled(false);
    QPushButton *closeButton = new QPushButton("Close", this);

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_trainButton);
    buttonsLayout->addWidget(closeButton);

    mainLayout->addWidget(inputBox);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(trainFileButton, SIGNAL(clicked()),
            this, SLOT(onTrainFileButtonClick()));
    connect(outputFileButton, SIGNAL(clicked()),
            this, SLOT(onOutputFileFuttonClick()));

    connect(m_trainFileEdit, SIGNAL(textChanged(QString)),
            this, SLOT(OnTextEdit()));
    connect(m_outputFileEdit, SIGNAL(textChanged(QString)),
            this, SLOT(OnTextEdit()));

    connect(m_trainButton, SIGNAL(clicked()), this, SLOT(onTrainButtonClick()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

MnistLearningForm::~MnistLearningForm() {
}

void MnistLearningForm::onTrainFileButtonClick() {
    QString path = QFileDialog::getOpenFileName(this,
        "Open MNIST Train File", QString(),
        "MNIST CSV Train File  (*.csv)");

    m_trainFileEdit->setText(path);
}

void MnistLearningForm::onOutputFileFuttonClick() {
    QString path = QFileDialog::getSaveFileName(this,
        "Save model file", QString(),
        "MNIST Trained model file (.json)");

    m_outputFileEdit->setText(path.isEmpty() ? path : path + ".json");
}

void MnistLearningForm::OnTextEdit() {
    // Enable train button
    if (m_trainFileEdit != nullptr && !m_trainFileEdit->text().isEmpty() &&
        m_outputFileEdit != nullptr && !m_outputFileEdit->text().isEmpty()) {
        m_trainButton->setEnabled(true);
    } else {
        m_trainButton->setEnabled(false);
    }
}

bool MnistLearningForm::saveModelToJson(const std::string& aFileName,
                                        const Perceptron& aNetwork) const {
    if (aFileName.empty()) {
        LOG_ERROR << "Empty JSON file name";
        return false;
    }

    if (std::filesystem::exists(aFileName)) {
        LOG_ERROR << "File " << aFileName << " already exists";
        return false;
    }

    if (!aNetwork.isConfigured()) {
        LOG_ERROR << "Network is not configured";
        return false;
    }

    if (!aNetwork.isTrained()) {
        LOG_ERROR << "Network is not trained";
        return false;
    }

    boost::json::object jsonModel;
    jsonModel["architecture"] = boost::json::array();
    jsonModel["layers"] = boost::json::array();

    // Fill architecture
    try {
        // Add first (input) layer
        if (!aNetwork.layers().empty()) {
            jsonModel["architecture"].as_array().emplace_back(kImageSize);
        }

        for (const auto& layer : aNetwork.layers()) {
            jsonModel["architecture"].as_array().emplace_back(
                static_cast<int>(layer.size()));
        }
    } catch (const std::exception& e) {
        LOG_ERROR << "Unable to fill JSON architecture with error " << e.what();
        return false;
    }

    // Fill layers and weights
    try {
        for (const auto& layer : aNetwork.layers()) {
            boost::json::object layerJson;
            layerJson["neurons"] = boost::json::array();

            for (const auto& neuron : layer) {
                boost::json::object neuronJson;
                neuronJson["bias"] = neuron.bias();
                neuronJson["weights"] = boost::json::array();

                for (const auto& weight : neuron.cweights()) {
                    neuronJson["weights"].as_array().emplace_back(weight);
                }

                layerJson["neurons"].as_array().emplace_back(neuronJson);
            }

            jsonModel["layers"].as_array().emplace_back(layerJson);
        }
    } catch (const std::exception& e) {
        LOG_ERROR << "Unable to fill layers and biases to JSON with error"
                  << e.what();
        return false;
    }

    // Save to JSON
    std::ofstream file(aFileName);
    if (!file.is_open()) {
        LOG_ERROR << "Unable to open file " << aFileName;
        return false;
    }

    try {
        file << boost::json::serialize(jsonModel) << std::endl;
    } catch (const std::exception& e) {
        LOG_ERROR << "Unable to write JSON to the file " << aFileName
                  << " with error " << e.what();
        return false;
    }

    file.close();

    LOG_INFO << "Model saved to " << aFileName;

    return true;
}

void MnistLearningForm::onTrainButtonClick() {
    m_trainButton->setEnabled(false);

    const std::string trainFile = m_trainFileEdit->text().toStdString();
    const std::string outputFile = m_outputFileEdit->text().toStdString();

    QFuture<void> future = QtConcurrent::run([=]() {
        std::vector<std::vector<double>> trainInputs;
        std::vector<std::vector<double>> trainTargets;

        LOG_INFO << "Loading MNIST data...";
        {
            MnistCsvDataSet trainSet(trainFile);
            if (!trainSet.isLoaded()) {
                QMetaObject::invokeMethod(this, [=]() {
                    QMessageBox::critical(this,
                        "Error", "Failed to load training data");
                    m_trainButton->setEnabled(true);
                }, Qt::QueuedConnection);
                return;
            }

            trainInputs.resize(trainSet.size());
            trainTargets.resize(trainSet.size());
            for (size_t i = 0; i < trainSet.size(); ++i) {
                trainTargets[i] = ToOneHot(trainSet[i].first);
                trainInputs[i] = NormalizeImage(trainSet[i].second);
            }
        }

        LOG_INFO << "Training started...";
        Perceptron network({kImageSize, 256, 128, kNumClasses},
                           Neuron::ActivationFunction::SIGMOID);
        try {
            network.train(trainInputs, trainTargets, 10, 0.0075);
        } catch (const std::exception& e) {
            QMetaObject::invokeMethod(this, [=]() {
                QMessageBox::critical(this, "Training Error", e.what());
                m_trainButton->setEnabled(true);
            }, Qt::QueuedConnection);
            return;
        }

        LOG_INFO << "Saving model...";
        if (!saveModelToJson(outputFile, network)) {
            QMetaObject::invokeMethod(this, [=]() {
                QMessageBox::critical(this, "Error", "Failed to save model");
                m_trainButton->setEnabled(true);
            }, Qt::QueuedConnection);
            return;
        }

        QMetaObject::invokeMethod(this, [=] () {
            QMessageBox::information(this,
                "Success", "Model saved successfully!");
            m_trainButton->setEnabled(true);
        }, Qt::QueuedConnection);
    });
}

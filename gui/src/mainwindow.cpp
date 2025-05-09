// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "include/mainwindow.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>

#include <string>
#include <filesystem>  // NOLINT(build/c++17)
#include <fstream>
#include <vector>

#include <boost/json.hpp>

#include "include/drawwidget.h"
#include "include/mnistlearningform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    // Draw box
    m_drawWidget = new DrawWidget();
    m_recButton = new QPushButton("Recognize", this);
    m_clearButton = new QPushButton("Clear", this);
    QHBoxLayout *drawButtonsLayout = new QHBoxLayout();
    drawButtonsLayout->addStretch();
    drawButtonsLayout->addWidget(m_recButton);
    drawButtonsLayout->addWidget(m_clearButton);

    QVBoxLayout *drawLayout = new QVBoxLayout();
    drawLayout->addWidget(m_drawWidget);
    drawLayout->addLayout(drawButtonsLayout);

    QGroupBox *drawBox = new QGroupBox("Input:", this);
    drawBox->setLayout(drawLayout);

    // Result box
    QVBoxLayout *resultLayout = new QVBoxLayout();
    for (int i = 0; i < kNumberClasses; ++i) {
        QLabel *label = new QLabel(QString::number(i));

        m_progressBars[i] = new QProgressBar();
        m_progressBars[i]->setMinimum(0);
        m_progressBars[i]->setMaximum(100);
        m_progressBars[i]->setValue(0);

        QHBoxLayout *resultProgressLayout = new QHBoxLayout();
        resultProgressLayout->addWidget(label);
        resultProgressLayout->addWidget(m_progressBars[i]);

        resultLayout->addLayout(resultProgressLayout);
    }

    QGroupBox *resultBox = new QGroupBox("Result:", this);
    resultBox->setLayout(resultLayout);

    // Setup central widget
    QHBoxLayout *vMainLayout = new QHBoxLayout();
    vMainLayout->addWidget(drawBox);
    vMainLayout->addWidget(resultBox);
    centralWidget->setLayout(vMainLayout);

    // Create main menu
    // Create File menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *openModelFileAction = new QAction("Open model file...");
    QAction *exitAction = new QAction("&Exit");

    fileMenu->addAction(openModelFileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // Create model menu
    QMenu *modelMenu = menuBar()->addMenu("&Model");
    QAction *learnModelAction = new QAction("Learn model");
    modelMenu->addAction(learnModelAction);

    // Create Help menu
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    QAction *aboutAction = new QAction("&About...");
    helpMenu->addAction(aboutAction);

    // Signals & slots
    // Buttons
    connect(m_clearButton, SIGNAL(clicked()), this,
            SLOT(onClearButtonClick()));
    connect(m_recButton, SIGNAL(clicked()), this,
            SLOT(onRecognizeButtonClick()));

    // Main menu
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(openModelFileAction, SIGNAL(triggered()), this,
            SLOT(onModelFileOpen()));
    connect(learnModelAction, SIGNAL(triggered()), this, SLOT(onLearnModel()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(onAbout()));
}

void MainWindow::onRecognizeButtonClick() {
    if (m_modelFileName.isEmpty()) {
        QMessageBox::warning(this,
                             "Recognition warning",
                             "Unable to recognize the number without"
                             "a learned model.\n\n"
                                   "Please, select model file.");
        return;
    }

    Perceptron network{};
    if (!loadModelFromJson(m_modelFileName, network)) {
        QMessageBox::warning(this,
                             "Training model warning",
                             "Unable to load model");
        return;
    }

    std::vector<double> imagePixels;
    m_drawWidget->getMnistCsvValues(imagePixels);

    std::vector<double> recResult = network.forward(imagePixels).back();

    for (int i = 0; i < kNumberClasses; ++i) {
        m_progressBars[i]->setValue(static_cast<int>(recResult[i] * 100));
    }

    return;
}

void MainWindow::onClearButtonClick() {
    m_drawWidget->clear();

    for (int i = 0; i < kNumberClasses; ++i) {
        m_progressBars[i]->setValue(0);
    }
}

void MainWindow::onModelFileOpen() {
    m_modelFileName = QFileDialog::getOpenFileName(
        this,
        "Select model file",
        QString(),
        "All files (*);;JSON file (*.json)");
}

void MainWindow::onLearnModel() {
    MnistLearningForm dialog;
    dialog.exec();
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About...",
                       "The application for number recognition");
}

bool MainWindow::loadModelFromJson(const QString& aFileName,
                                    Perceptron& aNetwork) const {
    std::string fileName = aFileName.toStdString();

    if (aFileName.isEmpty()) {
        return false;
    }

    if (!std::filesystem::exists(fileName)) {
        return false;
    }

    std::ifstream file(fileName);
    if (!file.is_open()) {
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    boost::json::value parsedJson;
    try {
        parsedJson = boost::json::parse(buffer.str());
    } catch (const std::exception& e) {
        return false;
    }

    boost::json::object jsonModel = parsedJson.as_object();
    // Check if JSON has required fields and types
    if (!jsonModel.contains("architecture") ||
        !jsonModel["architecture"].is_array() ||
        !jsonModel.contains("layers") ||
        !jsonModel["layers"].is_array()) {
        return false;
    }

    // Read architecture
    std::vector<size_t> architecture;
    try {
        for (const auto& layerSize : jsonModel["architecture"].as_array()) {
            if (!layerSize.is_number()) {
                return false;
            }

            architecture.emplace_back(layerSize.as_int64());
        }
    }
    catch(const std::exception& e) {
        return false;
    }

    if (architecture.empty()) {
        return false;
    }

    // Set architecture
    if (!aNetwork.initializeNetwork(architecture)) {
        return false;
    }

    const auto& jsonLayers = jsonModel["layers"].as_array();
    if (jsonLayers.size() != architecture.size() - 1) {
        return false;
    }

    // Read weights and biases
    try {
        for (size_t layerIndex = 0; layerIndex < jsonLayers.size();
             ++layerIndex) {
            const auto& jsonLayer = jsonLayers[layerIndex];
            for (size_t neuronIndex = 0;
                 neuronIndex <
                 jsonLayer.at("neurons").as_array().size();
                 ++neuronIndex) {
                const auto& neuronJson =
                    jsonLayer.at("neurons").as_array()[neuronIndex];

                const auto& weightsJson =
                    neuronJson.at("weights").as_array();
                std::vector<double> weights;
                for (const auto& weight : weightsJson) {
                    weights.emplace_back(weight.as_double());
                }

                aNetwork.setNeuronWeights(layerIndex, neuronIndex, weights);
                aNetwork.setNeuronBias(layerIndex, neuronIndex,
                                       neuronJson.at("bias").as_double());
            }
        }
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}

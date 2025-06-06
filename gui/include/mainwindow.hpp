// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef GUI_INCLUDE_MAINWINDOW_HPP_
#define GUI_INCLUDE_MAINWINDOW_HPP_

#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QString>

#include "include/perceptron.hpp"

class DrawWidget;

class MainWindow final : public QMainWindow {
    Q_OBJECT

 public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() {}

 private slots:
    // Main menu
    void onModelFileOpen();
    void onLearnModel();
    void onAbout();

    // Buttons
    void onRecognizeButtonClick();
    void onClearButtonClick();

 private:
    bool loadModelFromJson(const QString& aFileName,
                           // NOLINTNEXTLINE(runtime/references)
                           Perceptron& aNetwork) const;

 private:
    static constexpr int kNumberClasses = 10;  // numbers from 0 to 9
    QProgressBar *m_progressBars[kNumberClasses] = {};
    DrawWidget *m_drawWidget = nullptr;
    QPushButton *m_recButton = nullptr;
    QPushButton *m_clearButton = nullptr;

    QString m_modelFileName;
};

#endif  // GUI_INCLUDE_MAINWINDOW_HPP_

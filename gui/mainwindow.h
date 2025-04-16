// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef GUI_MAINWINDOW_H_
#define GUI_MAINWINDOW_H_

#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QString>

#include "include/perceptron.h"

class DrawWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() {}

 private slots:
    void onRecognizeButtonClick();
    void onClearButtonClick();
    void onModelFileOpen();
    void onAbout();

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

#endif  // GUI_MAINWINDOW_H_

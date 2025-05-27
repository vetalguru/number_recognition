// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef GUI_INCLUDE_MNISTLEARNINGFORM_HPP_
#define GUI_INCLUDE_MNISTLEARNINGFORM_HPP_

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include <string>
#include <vector>

class Perceptron;

class MnistLearningForm final : public QDialog {
    Q_OBJECT

 public:
    explicit MnistLearningForm(QWidget *parent = nullptr);
     virtual ~MnistLearningForm();

 private slots:
    void onTrainFileButtonClick();
    void onOutputFileFuttonClick();
    void onTrainButtonClick();
    void OnTextEdit();

 private:
    bool saveModelToJson(const std::string& aFileName,
                         const Perceptron& aNetwork) const;

 private:
    QLineEdit *m_trainFileEdit = nullptr;
    QLineEdit *m_outputFileEdit = nullptr;
    QPushButton *m_trainButton = nullptr;
};

#endif  // GUI_INCLUDE_MNISTLEARNINGFORM_HPP_

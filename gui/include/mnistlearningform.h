// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#ifndef GUI_INCLUDE_MNISTLEARNINGFORM_H_
#define GUI_INCLUDE_MNISTLEARNINGFORM_H_

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include <string>
#include <vector>

class Perceptron;

class MnistLearningForm : public QDialog {
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
    bool loadMnistCsv(const std::string& aFileName,
                      // NOLINTNEXTLINE(runtime/references)
                      std::vector<std::vector<double>>& aInputs,
                      // NOLINTNEXTLINE(runtime/references)
                      std::vector<std::vector<double>>& aTargets) const;

    bool saveModelToJson(const std::string& aFileName,
                         const Perceptron& aNetwork) const;

 private:
    QLineEdit *m_trainFileEdit = nullptr;
    QLineEdit *m_outputFileEdit = nullptr;
    QPushButton *m_trainButton = nullptr;
};

#endif  // GUI_INCLUDE_MNISTLEARNINGFORM_H_

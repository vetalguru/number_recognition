#include "mainwindow.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

#include "drawwidget.h"

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
    QVBoxLayout *resultLayout= new QVBoxLayout();
    for (int i = 0; i < kNumberClasses; ++i) {
        QLabel *label = new QLabel(QString::number(i));

        m_progressBars[i] = new QProgressBar();
        m_progressBars[i]->setMinimum(0);
        m_progressBars[i]->setMaximum(100);

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

    // Create Help menu
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    QAction *aboutAction = new QAction("&About...");
    helpMenu->addAction(aboutAction);

    // Signals & slots
    // Buttons
    connect(m_clearButton, SIGNAL(clicked()), m_drawWidget, SLOT(clear()));
    connect(m_recButton, SIGNAL(clicked()), this, SLOT(onRecognizeButtonClick()));

    // Main menu
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(openModelFileAction, SIGNAL(triggered()), this, SLOT(onModelFileOpen()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(onAbout()));
}

void MainWindow::onRecognizeButtonClick() {
}

void MainWindow::onModelFileOpen() {

}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About...", "The application for number recognition");
}

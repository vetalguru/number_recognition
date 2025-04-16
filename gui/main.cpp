// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include <QApplication>

#include "./mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

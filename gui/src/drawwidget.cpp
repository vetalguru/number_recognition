// Copyright (c) 2025 Vitalii Shkibtan. All rights reserved.

#include "include/drawwidget.hpp"

#include <QPainter>

namespace {
static constexpr int kMnistImageWidth = 28;
static constexpr int kMnistImageHeight = 28;
}

DrawWidget::DrawWidget(QWidget *parent)
    : QWidget(parent)
    , m_image(280, 280, QImage::Format_Grayscale8) {
    m_image.fill(Qt::white);
    setFixedSize(m_image.size());
}

void DrawWidget::clear() {
    m_image.fill(Qt::white);
    update();
}

void DrawWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, m_image);
}

void DrawWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_lastPoint = event->pos();
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPainter painter(&m_image);
        QPen pen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawLine(m_lastPoint, event->pos());
        m_lastPoint = event->pos();

        update();
    }
}

void DrawWidget::getMnistCsvValues(std::vector<double>& aOutput) const {
    aOutput.clear();

    QImage scaled = m_image.scaled(28, 28,
        Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    aOutput.reserve(kMnistImageWidth * kMnistImageHeight);

    for (int y = 0; y < kMnistImageHeight; ++y) {
        for (int x = 0; x < kMnistImageWidth; ++x) {
            double pixel = qGray(scaled.pixel(x, y));
            pixel = 255 - pixel;  // MNIST: 0 = background, 255 = black
            aOutput.emplace_back(pixel);
        }
    }
}

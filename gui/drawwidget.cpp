#include "drawwidget.h"

#include <QPainter>
#include <QStringList>

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

QString DrawWidget::getMnistCsvString(int aLabel) const {
    QImage scaled = m_image.scaled(28, 28, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QStringList values;
    values << QString::number(aLabel);

    for (int y = 0; y < kMnistImageHeight; ++y) {
        for (int x = 0; x < kMnistImageWidth; ++x) {
            int pixel = qGray(scaled.pixel(x, y));
            pixel = 255 - pixel;  // MNIST: 0 = background, 255 = black
            values << QString::number(pixel);
        }
    }

    return values.join(kMnistDelimiter);
}

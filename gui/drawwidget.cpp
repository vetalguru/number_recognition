#include "drawwidget.h"

#include <QPainter>

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

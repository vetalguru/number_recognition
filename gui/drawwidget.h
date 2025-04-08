#ifndef GUI_DRAWWIDGET_H
#define GUI_DRAWWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QPaintEvent>
#include <QMouseEvent>

class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawWidget(QWidget *parent = nullptr);
    virtual ~DrawWidget() {}

public slots:
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QImage m_image;
    QPoint m_lastPoint;
};

#endif // GUI_DRAWWIDGET_H

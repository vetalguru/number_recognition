#ifndef GUI_DRAWWIDGET_H
#define GUI_DRAWWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QString>

#include <vector>

class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawWidget(QWidget *parent = nullptr);
    virtual ~DrawWidget() {}

    void getMnistCsvValues(std::vector<double>& aOutput) const;

public slots:
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    static constexpr char kMnistDelimiter = ',';
    static constexpr int kMnistImageWidth = 28;
    static constexpr int kMnistImageHeight = 28;

    QImage m_image;
    QPoint m_lastPoint;
};

#endif // GUI_DRAWWIDGET_H

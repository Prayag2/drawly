#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    QImage* const canvas() const;
    QImage* const overlay() const;
    QSize sizeHint() const override;

    QColor bg() const;
    void setBg(const QColor& color, QImage *canvas = nullptr, QImage *overlay = nullptr);

    qreal scale() const;
    void setScale(const qreal scale);

signals:
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void resizeStart();
    void resizeEnd();
    void destroyed();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    qreal m_scale {1.0}; // default scale is 1
    QImage *m_canvas {};
    QImage *m_overlay {};
    QColor m_bg {};

    QSize m_sizeHint {500, 500};
    QSize m_maxSize {};
    const QImage::Format m_imageFormat {QImage::Format_ARGB32};

    static QByteArray getImageData(QImage* const img);
    static void setImageData(QImage *const img, const QByteArray& arr);
    void resize();
};

#endif // CANVAS_H

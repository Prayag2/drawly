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
    void setBg(const QColor& color);

signals:
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QImage *m_canvas {};
    QImage *m_overlay {};
    QColor m_bg {};

    QSize m_sizeHint {500, 500};
    const QImage::Format m_imageFormat {QImage::Format_ARGB32};

    static QByteArray getImageData(QImage* const img);
    static void setImageData(QImage *const img, const QByteArray& arr);
};

#endif // CANVAS_H

#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <QList>
#include <QPixmap>
#include "../shapes/shape.h"
#include "../data-structures/quadtree.h"

class Canvas : public QWidget
{
Q_OBJECT
public:
    Canvas(QWidget* parent = nullptr);
    void setShape(ShapeType shapeType);
    ShapeType shape() const;
private:
    void paintEvent(QPaintEvent *event) override;
    QImage* m_canvas {};
    QImage* m_overlayCanvas {};
    QPainter m_canvasPainter {};
    QPainter m_overlayPainter {};
    QPainter m_widgetPainter {};
    QPen m_pen {};
    QPen m_overlayResetPen {};
    QuadTree* m_shapes {};

    QRect m_dirtyRegion {};
    QVector<Shape*> m_dirtyShapes {};
    Shape* m_curShape {nullptr};
    ShapeType m_shape {};

    QCursor m_eraser {};

    bool m_isDrawing {false};
    double m_scale {};
    int m_width {800};
    int m_height {800};

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;

    QVector<Shape*> getDirtyShapes(const QRect& boundingBox) const;
};

#endif // CANVAS_H

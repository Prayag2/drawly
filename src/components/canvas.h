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
    QImage* m_canvas {};
    QImage* m_overlay {};

    QPainter m_canvasPainter {};
    QPainter m_overlayPainter {};
    QPainter m_widgetPainter {};

    QPen m_pen {};
    QPen m_overlayResetPen {};
    QPen m_selectionPen {};
    QuadTree* m_shapeTree {};

    ShapeType m_curShapeType {};
    Shape* m_curDrawnShape {nullptr};
    QCursor m_eraser {};

    bool m_isDrawing {false};

    QPoint m_topLeftPosition {0, 0};
    double m_scale {};
    int m_width {500};
    int m_height {500};
    QSize m_initialScreenSize {};

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    QImage* m_createImage(QColor fill) const;

    void m_startCanvasPainter();
    void m_startOverlayPainter();
    void m_endCanvasPainter();
    void m_endOverlayPainter();
};

#endif // CANVAS_H

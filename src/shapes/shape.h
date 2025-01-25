#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>

enum class DrawingMode {
    Drag = 0x00,
    Sketch = 0x02,
    Erase = 0x04
};

enum class ShapeType {
    Line = 0x00,
    Rectangle = 0x02,
    Ellipse = 0x04,
    Arrow = 0x08,
    Stroke = 0x10,
    Eraser = 0x20,
    Selection = 0x40,
};

class Shape
{
public:
    Shape(int strokeWidth);

    virtual DrawingMode drawingMode() const = 0;
    virtual const QRect& boundingBox() const = 0;
    virtual ShapeType shapeType() const = 0;
    virtual bool intersects(QRect boundingBox) const = 0;
    virtual void draw(QPainter& painter) const = 0;

    static Shape* createShape(ShapeType shapeType, int strokeWidth);

protected:
    QRect m_boundingBox {};
    int m_strokeWidth {};
    int m_margin {};

    static int orientation(QPoint a, QPoint b, QPoint c);
    static bool linesIntersect(QLine a, QLine b);
};

#endif // SHAPE_H

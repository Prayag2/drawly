#ifndef ERASER_H
#define ERASER_H

#include "shape.h"
#include <QCursor>

class Eraser : public Shape
{
public:
    Eraser();

    DrawingMode drawingMode() const override;
    const QRect& boundingBox() const override;
    ShapeType shapeType() const override;
    bool intersects(QRect boundingBox) const override;
    void draw(QPainter& painter) const override;

    void setBoundingBox(QRect boundingBox);
    static QCursor createCursor (int size, int borderWidth);
};

#endif // ERASER_H

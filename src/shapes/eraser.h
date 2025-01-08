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

    void setBoundingBox(QRect boundingBox);
    QCursor createCursor (int size, int borderWidth) const;
};

#endif // ERASER_H

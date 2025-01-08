#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "draggableshape.h"

class Rectangle: public DraggableShape
{
public:
    Rectangle(int strokeWidth);
    Rectangle(const Rectangle& rectangle);
    Rectangle(Rectangle* const rectangle);
    Rectangle& operator=(const Rectangle& rectangle);

    void draw(QPainter& painter) const override;
    bool intersects(QRect boundingBox) const override;
    ShapeType shapeType() const override;
};

#endif // RECTANGLE_H

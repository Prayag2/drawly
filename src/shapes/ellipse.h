#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "draggableshape.h"

class Ellipse: public DraggableShape
{
public:
    Ellipse(int strokeWidth);
    Ellipse(const Ellipse& ellipse);
    Ellipse(Ellipse* const ellipse);
    Ellipse& operator=(const Ellipse& ellipse);

    void draw(QPainter& painter) const override;
    bool intersects(QRect boundingBox) const override;
    ShapeType shapeType() const override;
private:
    bool onEllipse(QLine line) const;
};

#endif // ELLIPSE_H

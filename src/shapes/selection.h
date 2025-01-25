#ifndef SELECTION_H
#define SELECTION_H

#include "draggableshape.h"

class Selection : public DraggableShape
{
public:
    Selection();
    Selection(const Selection& selection);
    Selection(Selection* const selection);
    Selection& operator=(const Selection& selection);

    DrawingMode drawingMode() const override;
    const QRect& boundingBox() const override;
    ShapeType shapeType() const override;
    bool intersects(QRect boundingBox) const override;
    void draw(QPainter& painter) const override;
};

#endif // SELECTION_H

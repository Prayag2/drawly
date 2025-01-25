#ifndef DRAGGABLESHAPE_H
#define DRAGGABLESHAPE_H

#include "shape.h"

class DraggableShape : public Shape
{
public:
    DraggableShape(int strokeWidth);

    virtual void setStart(QPoint start);
    virtual void setEnd(QPoint end);

    const QPoint& start();
    const QPoint& end();
    const QRect& boundingBox() const override;

    DrawingMode drawingMode() const override;

protected:
    QPoint m_start {};
    QPoint m_end {};

private:
    void updateBoundingBox();
};

#endif // DRAGGABLESHAPE_H

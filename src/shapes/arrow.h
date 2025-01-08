#ifndef ARROW_H
#define ARROW_H

#include "draggableshape.h"

class Arrow: public DraggableShape
{
public:
    Arrow(int strokeWidth);
    Arrow(const Arrow& arrow);
    Arrow(Arrow* const arrow);
    Arrow& operator=(const Arrow& arrow);

    void draw(QPainter& painter) const override;
    bool intersects(QRect boundingBox) const override;
    ShapeType shapeType() const override;

    void setStart(QPoint start) override;
    void setEnd(QPoint end) override;
private:
    int m_arrowHeadSize {20}; // pixels
    double m_arrowHeadAngle {30}; // degrees
    void calcArrowPoints();

    QPoint m_arrowP1 {};
    QPoint m_arrowP2 {};
};

#endif // ARROW_H

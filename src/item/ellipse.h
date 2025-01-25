#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "polygon.h"

class Ellipse : public Polygon
{
public:
    Ellipse();
    virtual ~Ellipse() = default;

    void draw(QPainter& painter) const override;
    bool intersects(const QRect& rect) override;
};

#endif // ELLIPSE_H

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "polygon.h"

class Rectangle : public Polygon
{
public:
    Rectangle();
    ~Rectangle() override = default;

    void draw(QPainter& painter) const override;
    bool intersects(const QRect& rect) override;
};

#endif // RECTANGLE_H

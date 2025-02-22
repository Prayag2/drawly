#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "polygon.h"

class Ellipse : public Polygon
{
public:
    Ellipse();
    virtual ~Ellipse() = default;

    void draw(QPainter& painter, const QPoint& offset) const override;
    bool intersects(const QRect& rect) override;

private:
    bool onEllipse(QLine line) const;
};

#endif // ELLIPSE_H

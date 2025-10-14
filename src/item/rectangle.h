#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "polygon.h"

class Rectangle : public Polygon {
public:
    Rectangle();
    ~Rectangle() override = default;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    Item::Type type() const override;

protected:
    void m_draw(QPainter &painter, const QPointF &offset) const override;
};

#endif  // RECTANGLE_H

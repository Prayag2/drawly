#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "polygon.h"

class Ellipse : public Polygon {
public:
    Ellipse();
    virtual ~Ellipse() = default;

    bool intersects(const QRectF& rect) override;
    bool intersects(const QLineF& rect) override;

protected:
    void m_draw(QPainter& painter, const QPointF& offset) const override;

private:
    bool onEllipse(QLineF line) const;
};

#endif  // ELLIPSE_H

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "polygon.h"

class Rectangle : public Polygon {
  public:
    Rectangle();
    ~Rectangle() override = default;

    bool intersects(const QRect& rect) override;

  protected:
    void m_draw(QPainter& painter, const QPoint& offset) const override;
};

#endif // RECTANGLE_H

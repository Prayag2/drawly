#ifndef LINE_H
#define LINE_H

#include "polygon.h"

class Line : public Polygon {
  public:
    Line();
    ~Line() override = default;

    void draw(QPainter& painter, const QPoint& offset) const override;
    bool intersects(const QRect& rect) override;
};

#endif // LINE_H

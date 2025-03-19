#ifndef LINE_H
#define LINE_H

#include "polygon.h"

class Line : public Polygon {
  public:
    Line();
    ~Line() override = default;

    bool intersects(const QRect& rect) override;

private:
    void m_draw(QPainter& painter, const QPoint& offset) const override;
};

#endif // LINE_H

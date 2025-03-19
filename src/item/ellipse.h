#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "polygon.h"

class Ellipse : public Polygon {
  public:
    Ellipse();
    virtual ~Ellipse() = default;

    bool intersects(const QRect& rect) override;

protected:
    void m_draw(QPainter& painter, const QPoint& offset) const override;

  private:
    bool onEllipse(QLine line) const;
};

#endif // ELLIPSE_H

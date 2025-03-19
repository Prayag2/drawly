#ifndef ARROW_H
#define ARROW_H

#include "polygon.h"

class Arrow : public Polygon {
  public:
    Arrow();
    ~Arrow() override = default;

    void setStart(QPoint start) override;
    void setEnd(QPoint end) override;

    bool intersects(const QRect& rect) override;

  protected:
    void m_draw(QPainter& painter, const QPoint& offset) const override;

  private:
    QPoint m_arrowP1;
    QPoint m_arrowP2;

    int m_maxArrowSize {15}; // hardcoded for now

    void calcArrowPoints();
};

#endif // ARROW_H

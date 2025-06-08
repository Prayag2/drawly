#ifndef ARROW_H
#define ARROW_H

#include "polygon.h"

class Arrow : public Polygon {
public:
    Arrow();
    ~Arrow() override = default;

    void setStart(QPointF start) override;
    void setEnd(QPointF end) override;

    bool intersects(const QRectF& rect) override;
    bool intersects(const QLineF& rect) override;

protected:
    void m_draw(QPainter& painter, const QPointF& offset) const override;

private:
    QPointF m_arrowP1;
    QPointF m_arrowP2;

    int m_maxArrowSize{15};  // hardcoded for now

    void calcArrowPoints();
};

#endif  // ARROW_H

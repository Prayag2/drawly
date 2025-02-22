#ifndef ARROW_H
#define ARROW_H

#include "polygon.h"

class Arrow : public Polygon
{
public:
    Arrow();
    ~Arrow() override = default;

    void setStart(QPoint start) override;
    void setEnd(QPoint end) override;

    void draw(QPainter& painter, const QPoint& offset) const override;
    bool intersects(const QRect& rect) override;

private:
    QPoint m_arrowP1;
    QPoint m_arrowP2;

    int m_maxArrowSize {15}; // hardcoded for now

    void calcArrowPoints();
};

#endif // ARROW_H

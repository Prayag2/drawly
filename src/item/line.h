#ifndef LINE_H
#define LINE_H

#include "polygon.h"

class LineItem : public PolygonItem {
public:
    LineItem();
    ~LineItem() override = default;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    Item::Type type() const override;

private:
    void m_draw(QPainter &painter, const QPointF &offset) const override;
};

#endif  // LINE_H

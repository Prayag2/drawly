#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "polygon.h"

class EllipseItem : public PolygonItem {
public:
    EllipseItem();
    virtual ~EllipseItem() = default;

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    Item::Type type() const override;

protected:
    void m_draw(QPainter &painter, const QPointF &offset) const override;

private:
    bool onEllipse(QLineF line) const;
};

#endif  // ELLIPSE_H

#include "rectangle.h"
#include <QDebug>

Rectangle::Rectangle() {}

void Rectangle::m_draw(QPainter& painter, const QPointF& offset) const {
    painter.drawRect(QRectF(start() - offset, end() - offset));
}

bool Rectangle::intersects(const QRectF& rect) {
    if (!boundingBox().intersects(rect)) return false;

    int mg {m_boundingBoxPadding + getProperty(ItemPropertyType::StrokeWidth).value().toInt()};
    QRectF box {boundingBox().normalized().adjusted(mg, mg, -mg, -mg)};
    QPointF p {box.topLeft()};
    QPointF q {box.topRight()};
    QPointF r {box.bottomRight()};
    QPointF s {box.bottomLeft()};

    QPointF a {rect.topLeft()};
    QPointF b {rect.topRight()};
    QPointF c {rect.bottomRight()};
    QPointF d {rect.bottomLeft()};

    return (
               Item::linesIntersect({p, q}, {a, b}) ||
               Item::linesIntersect({p, q}, {b, c}) ||
               Item::linesIntersect({p, q}, {c, d}) ||
               Item::linesIntersect({p, q}, {d, a}) ||
               Item::linesIntersect({q, r}, {a, b}) ||
               Item::linesIntersect({q, r}, {b, c}) ||
               Item::linesIntersect({q, r}, {c, d}) ||
               Item::linesIntersect({q, r}, {d, a}) ||
               Item::linesIntersect({r, s}, {a, b}) ||
               Item::linesIntersect({r, s}, {b, c}) ||
               Item::linesIntersect({r, s}, {c, d}) ||
               Item::linesIntersect({r, s}, {d, a}) ||
               Item::linesIntersect({p, s}, {a, b}) ||
               Item::linesIntersect({p, s}, {b, c}) ||
               Item::linesIntersect({p, s}, {c, d}) ||
               Item::linesIntersect({p, s}, {d, a})
           );
};

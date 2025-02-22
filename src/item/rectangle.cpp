#include "rectangle.h"
#include <QDebug>

Rectangle::Rectangle() {}

void Rectangle::draw(QPainter& painter, const QPoint& offset) const {
    painter.drawRect(QRect(start()+offset, end()+offset));
}

bool Rectangle::intersects(const QRect& rect) {
    if (!boundingBox().intersects(rect)) return false;

    int mg {m_boundingBoxPadding + stroke().width()};
    QRect box {boundingBox().normalized().adjusted(mg, mg, -mg, -mg)};
    QPoint p {box.topLeft()};
    QPoint q {box.topRight()};
    QPoint r {box.bottomRight()};
    QPoint s {box.bottomLeft()};

    QPoint a {rect.topLeft()};
    QPoint b {rect.topRight()};
    QPoint c {rect.bottomRight()};
    QPoint d {rect.bottomLeft()};

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

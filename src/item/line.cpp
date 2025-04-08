#include "line.h"

Line::Line() {}

void Line::m_draw(QPainter& painter, const QPointF& offset) const {
    painter.drawLine(start() - offset, end() - offset);
}

bool Line::intersects(const QRectF& rect) {
    if (!boundingBox().intersects(rect)) return false;

    QPointF x {start()}, y {end()};
    QPointF a {rect.x(), rect.y()};
    QPointF b {rect.x()+rect.width(), rect.y()};
    QPointF c {rect.x()+rect.width(), rect.y()+rect.height()};
    QPointF d {rect.x(), rect.y()+rect.height()};

    if (Polygon::orientation(x, y, a) != Polygon::orientation(x, y, b) && Polygon::orientation(a, b, x) != Polygon::orientation(a, b, y))
        return true;
    if (Polygon::orientation(x, y, b) != Polygon::orientation(x, y, c) && Polygon::orientation(b, c, x) != Polygon::orientation(b, c, y))
        return true;
    if (Polygon::orientation(x, y, c) != Polygon::orientation(x, y, d) && Polygon::orientation(c, d, x) != Polygon::orientation(c, d, y))
        return true;
    if (Polygon::orientation(x, y, d) != Polygon::orientation(x, y, a) && Polygon::orientation(d, a, x) != Polygon::orientation(d, a, y))
        return true;
    return false;
};

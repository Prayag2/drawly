#include "line.h"

Line::Line() {}

void Line::draw(QPainter& painter, const QPoint& offset) const {
    painter.drawLine(start()+offset, end()+offset);
}

bool Line::intersects(const QRect& rect) {
    if (!boundingBox().intersects(rect)) return false;

    QPoint x {start()}, y {end()};
    QPoint a {rect.x(), rect.y()};
    QPoint b {rect.x()+rect.width(), rect.y()};
    QPoint c {rect.x()+rect.width(), rect.y()+rect.height()};
    QPoint d {rect.x(), rect.y()+rect.height()};

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

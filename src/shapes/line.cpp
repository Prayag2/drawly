#include "line.h"
#include <QDebug>

Line::Line(int strokeWidth) : DraggableShape (strokeWidth) {}
Line::Line(const Line& line) : DraggableShape (line.m_margin) {
    m_start = line.m_start;
    m_end = line.m_end;
    m_boundingBox = line.m_boundingBox;
}

Line::Line(Line* const line) : DraggableShape (line->m_margin) {
    m_start = line->m_start;
    m_end = line->m_end;
    m_boundingBox = line->m_boundingBox;
}

Line& Line::operator=(const Line& line) {
    if (this == &line)
        return *this;

    m_boundingBox = line.m_boundingBox;
    m_start = line.m_start;
    m_end = line.m_end;
    m_margin = line.m_margin;
    return *this;
}

void Line::draw(QPainter& painter) const {
    painter.drawLine(m_start, m_end);
}

ShapeType Line::shapeType() const {
    return ShapeType::Line;
}

int Line::orientation(QPoint a, QPoint b, QPoint c) const {
    QPoint ab {b.x()-a.x(), b.y()-a.y()};
    QPoint ac {c.x()-a.x(), c.y()-a.y()};
    int orient {ab.x()*ac.y()-ac.x()*ab.y()};
    return (orient == 0 ? 0 : (orient < 0 ? -1 : 1));
}

bool Line::intersects(QRect boundingBox) const {
    if (!m_boundingBox.intersects(boundingBox)) return false;
    QPoint x {m_start}, y {m_end};
    QPoint a {boundingBox.x(), boundingBox.y()};
    QPoint b {boundingBox.x()+boundingBox.width(), boundingBox.y()};
    QPoint c {boundingBox.x()+boundingBox.width(), boundingBox.y()+boundingBox.height()};
    QPoint d {boundingBox.x(), boundingBox.y()+boundingBox.height()};

    if (orientation(x, y, a) != orientation(x, y, b) && orientation(a, b, x) != orientation(a, b, y))
        return true;
    if (orientation(x, y, b) != orientation(x, y, c) && orientation(b, c, x) != orientation(b, c, y))
        return true;
    if (orientation(x, y, c) != orientation(x, y, d) && orientation(c, d, x) != orientation(c, d, y))
        return true;
    if (orientation(x, y, d) != orientation(x, y, a) && orientation(d, a, x) != orientation(d, a, y))
        return true;
    return false;
}




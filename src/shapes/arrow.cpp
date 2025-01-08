#include "arrow.h"
#include <cmath>

Arrow::Arrow(int strokeWidth) : DraggableShape (strokeWidth) {}
Arrow::Arrow(const Arrow& arrow) : DraggableShape (arrow.m_margin) {
    m_start = arrow.m_start;
    m_end = arrow.m_end;
    m_boundingBox = arrow.m_boundingBox;
}

Arrow::Arrow(Arrow* const arrow) : DraggableShape (arrow->m_margin) {
    m_start = arrow->m_start;
    m_end = arrow->m_end;
    m_boundingBox = arrow->m_boundingBox;
}

Arrow& Arrow::operator=(const Arrow& arrow) {
    if (this == &arrow)
        return *this;

    m_boundingBox = arrow.m_boundingBox;
    m_start = arrow.m_start;
    m_end = arrow.m_end;
    m_margin = arrow.m_margin;
    return *this;
}

void Arrow::calcArrowPoints() {
    int x1 = m_start.x(), x2 = m_end.x();
    int y1 = m_start.y(), y2 = m_end.y();

    double angle {std::atan2(y2-y1, x2-x1)};
    double arrowLength {std::sqrt(std::pow(y2-y1, 2)+std::pow(x2-x1,2))};
    int arrowSize = std::min(m_arrowHeadSize, static_cast<int>(arrowLength*0.5));

    m_arrowP1 = QPoint(x2 - arrowSize * std::cos(angle - (M_PI / 180) * m_arrowHeadAngle),
                       y2 - arrowSize * std::sin(angle - (M_PI / 180) * m_arrowHeadAngle));
    m_arrowP2 = QPoint(x2 - arrowSize * std::cos(angle + (M_PI / 180) * m_arrowHeadAngle),
                       y2 - arrowSize * std::sin(angle + (M_PI / 180) * m_arrowHeadAngle));
}

void Arrow::setStart(QPoint start) {
    DraggableShape::setStart(start);
    calcArrowPoints();
}

void Arrow::setEnd(QPoint end) {
    DraggableShape::setEnd(end);
    calcArrowPoints();
}

void Arrow::draw(QPainter& painter) const {
    painter.drawLine(m_start, m_end);
    painter.drawLine(m_end, m_arrowP1);
    painter.drawLine(m_end, m_arrowP2);
}

ShapeType Arrow::shapeType() const {
    return ShapeType::Arrow;
}

bool Arrow::intersects(QRect boundingBox) const {
    if (!m_boundingBox.intersects(boundingBox)) return false;
    QPoint p {m_start}, q {m_end}, r {m_arrowP1}, s {m_arrowP2};
    QPoint a {boundingBox.x(), boundingBox.y()};
    QPoint b {boundingBox.x()+boundingBox.width(), boundingBox.y()};
    QPoint c {boundingBox.x()+boundingBox.width(), boundingBox.y()+boundingBox.height()};
    QPoint d {boundingBox.x(), boundingBox.y()+boundingBox.height()};

    return (
        Shape::linesIntersect({p, q}, {a, b}) ||
        Shape::linesIntersect({p, q}, {b, c}) ||
        Shape::linesIntersect({p, q}, {c, d}) ||
        Shape::linesIntersect({p, q}, {d, a}) ||
        Shape::linesIntersect({q, r}, {a, b}) ||
        Shape::linesIntersect({q, r}, {b, c}) ||
        Shape::linesIntersect({q, r}, {c, d}) ||
        Shape::linesIntersect({q, r}, {d, a}) ||
        Shape::linesIntersect({q, s}, {a, b}) ||
        Shape::linesIntersect({q, s}, {b, c}) ||
        Shape::linesIntersect({q, s}, {c, d}) ||
        Shape::linesIntersect({q, s}, {d, a})
    );
}

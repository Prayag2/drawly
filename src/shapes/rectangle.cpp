#include "rectangle.h"

Rectangle::Rectangle(int strokeWidth) : DraggableShape (strokeWidth) {}
Rectangle::Rectangle(const Rectangle& rectangle) : DraggableShape (rectangle.m_margin) {
    m_start = rectangle.m_start;
    m_end = rectangle.m_end;
    m_boundingBox = rectangle.m_boundingBox;
}

Rectangle::Rectangle(Rectangle* const rectangle) : DraggableShape (rectangle->m_margin) {
    m_start = rectangle->m_start;
    m_end = rectangle->m_end;
    m_boundingBox = rectangle->m_boundingBox;
}

Rectangle& Rectangle::operator=(const Rectangle& rectangle) {
    if (this == &rectangle)
        return *this;

    m_boundingBox = rectangle.m_boundingBox;
    m_start = rectangle.m_start;
    m_end = rectangle.m_end;
    m_margin = rectangle.m_margin;
    return *this;
}

void Rectangle::draw(QPainter& painter) const {
    int mg {m_margin};
    painter.drawRect(this->boundingBox().adjusted(mg, mg, -mg, -mg));
}

ShapeType Rectangle::shapeType() const {
    return ShapeType::Rectangle;
}

bool Rectangle::intersects(QRect boundingBox) const {
    if (!m_boundingBox.intersects(boundingBox)) return false;

    int mg {m_margin};
    QRect box {m_boundingBox.normalized().adjusted(mg, mg, -mg, -mg)};
    QPoint p {box.topLeft()};
    QPoint q {box.topRight()};
    QPoint r {box.bottomRight()};
    QPoint s {box.bottomLeft()};

    QPoint a {boundingBox.topLeft()};
    QPoint b {boundingBox.topRight()};
    QPoint c {boundingBox.bottomRight()};
    QPoint d {boundingBox.bottomLeft()};

    return (
        Shape::linesIntersect({p, q}, {a, b}) ||
        Shape::linesIntersect({p, q}, {b, c}) ||
        Shape::linesIntersect({p, q}, {c, d}) ||
        Shape::linesIntersect({p, q}, {d, a}) ||
        Shape::linesIntersect({q, r}, {a, b}) ||
        Shape::linesIntersect({q, r}, {b, c}) ||
        Shape::linesIntersect({q, r}, {c, d}) ||
        Shape::linesIntersect({q, r}, {d, a}) ||
        Shape::linesIntersect({r, s}, {a, b}) ||
        Shape::linesIntersect({r, s}, {b, c}) ||
        Shape::linesIntersect({r, s}, {c, d}) ||
        Shape::linesIntersect({r, s}, {d, a}) ||
        Shape::linesIntersect({p, s}, {a, b}) ||
        Shape::linesIntersect({p, s}, {b, c}) ||
        Shape::linesIntersect({p, s}, {c, d}) ||
        Shape::linesIntersect({p, s}, {d, a})
    );

    // this formula created some inconsistencies idk why
    // so, im not using it
    // return !((x > X) && (x+w < X+W) && (y > Y) && (y+h < Y+H));
}

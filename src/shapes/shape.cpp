#include "shape.h"

#include "line.h"
#include "rectangle.h"
#include "selection.h"
#include "eraser.h"
#include "ellipse.h"
#include "arrow.h"
#include "stroke.h"
#include <QDebug>

Shape::Shape(int strokeWidth) : m_margin {2*strokeWidth}, m_strokeWidth {strokeWidth} {}

Shape* Shape::createShape(ShapeType shapeType, int strokeWidth) {
    Shape *shape {nullptr};

    switch(shapeType) {
    case ShapeType::Line:
        shape = new Line(strokeWidth);
        break;
    case ShapeType::Ellipse:
        shape = new Ellipse(strokeWidth);
        break;
    case ShapeType::Rectangle:
        shape = new Rectangle(strokeWidth);
        break;
    case ShapeType::Arrow:
        shape = new Arrow(strokeWidth);
        break;
    case ShapeType::Eraser:
        shape = new Eraser();
        break;
    case ShapeType::Selection:
        shape = new Selection();
        break;
    case ShapeType::Stroke:
    default:
        shape = new Stroke(strokeWidth);
    }

    return shape;
}

int Shape::orientation(QPoint a, QPoint b, QPoint c) {
    QPoint ab {b.x()-a.x(), b.y()-a.y()};
    QPoint ac {c.x()-a.x(), c.y()-a.y()};
    int orient {ab.x()*ac.y()-ac.x()*ab.y()};
    return (orient == 0 ? 0 : (orient < 0 ? -1 : 1));
}

bool Shape::linesIntersect(QLine a, QLine b) {
    QPoint p {a.p1()}, q {a.p2()};
    QPoint r {b.p1()}, s {b.p2()};
    return orientation(p, q, r) != orientation(p, q, s) && orientation(r, s, p) != orientation(r, s, q);
}

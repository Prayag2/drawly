#include "ellipse.h"

#include <QDebug>
Ellipse::Ellipse(int strokeWidth) : DraggableShape (strokeWidth) {}
Ellipse::Ellipse(const Ellipse& ellipse) : DraggableShape (ellipse.m_margin) {
    m_start = ellipse.m_start;
    m_end = ellipse.m_end;
    m_boundingBox = ellipse.m_boundingBox;
}

Ellipse::Ellipse(Ellipse* const ellipse) : DraggableShape (ellipse->m_margin) {
    m_start = ellipse->m_start;
    m_end = ellipse->m_end;
    m_boundingBox = ellipse->m_boundingBox;
}

Ellipse& Ellipse::operator=(const Ellipse& ellipse) {
    if (this == &ellipse)
        return *this;

    m_boundingBox = ellipse.m_boundingBox;
    m_start = ellipse.m_start;
    m_end = ellipse.m_end;
    m_margin = ellipse.m_margin;
    return *this;
}


void Ellipse::draw(QPainter& painter) const {
    int w {m_margin};
    painter.drawEllipse(this->boundingBox().adjusted(w, w, -w, -w));
}

ShapeType Ellipse::shapeType() const {
    return ShapeType::Ellipse;
}

bool Ellipse::onEllipse(QLine line) const {
    int sw = m_margin;
    int X = m_boundingBox.x() + sw, Y = m_boundingBox.y() + sw;
    int W = m_boundingBox.width() - 2*sw, H = m_boundingBox.height() - 2*sw;

    int h = X + W/2, k = Y + H/2;
    int a = W/2, b = H/2;
    int x1 = line.x1(), y1 = line.y1();
    int x2 = line.x2(), y2 = line.y2();

    int p = x2-x1, q = y2-y1;

    long long as = static_cast<long long>(a)*a;
    long long bs = static_cast<long long>(b)*b;
    long long ps = static_cast<long long>(p)*p;
    long long qs = static_cast<long long>(q)*q;

    long long A = ps*bs + qs*as;
    long long B = 2 * (x1*p*bs - p*h*bs + y1*q*as - q*k*as);
    long long C = static_cast<long long>(x1)*x1*bs + bs*h*h - 2*x1*h*bs +
                  static_cast<long long>(y1)*y1*as + as*k*k - 2*y1*k*as - as*bs;

    long long discriminant = B*B - 4*A*C;
    if (discriminant < 0) return false;

    double t1 = (-B + sqrt(discriminant))/(2.0*A);
    double t2 = (-B - sqrt(discriminant))/(2.0*A);

    return (t1>=0.0 && t1<=1.0) || (t2>=0.0 && t2<=1.0);
}

bool Ellipse::intersects(QRect boundingBox) const {
    if (!m_boundingBox.intersects(boundingBox)) return false;

    QPoint a {boundingBox.topLeft()};
    QPoint b {boundingBox.topRight()};
    QPoint c {boundingBox.bottomRight()};
    QPoint d {boundingBox.bottomLeft()};
    return onEllipse({a, b}) || onEllipse({b, c}) || onEllipse({c, d}) || onEllipse({d, a});
}

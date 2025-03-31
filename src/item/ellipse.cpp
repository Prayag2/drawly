#include "ellipse.h"

Ellipse::Ellipse() {}

void Ellipse::m_draw(QPainter& painter, const QPoint& offset) const {
    painter.drawEllipse(QRect(start()-offset, end()-offset));
}

bool Ellipse::onEllipse(QLine line) const {
    int sw {m_boundingBoxPadding + getProperty(ItemPropertyType::StrokeWidth).value().toInt()};
    int X {m_boundingBox.x() + sw}, Y {m_boundingBox.y() + sw};
    int W {m_boundingBox.width() - 2*sw}, H {m_boundingBox.height() - 2*sw};

    int h {X + W/2}, k {Y + H/2};
    int a {W/2}, b {H/2};
    int x1 {line.x1()}, y1 {line.y1()};
    int x2 {line.x2()}, y2 {line.y2()};

    int p {x2-x1}, q {y2-y1};

    long long as {static_cast<long long>(a)*a};
    long long bs {static_cast<long long>(b)*b};
    long long ps {static_cast<long long>(p)*p};
    long long qs {static_cast<long long>(q)*q};

    long long A {ps*bs + qs*as};
    long long B {2*(x1*p*bs - p*h*bs + y1*q*as - q*k*as)};
    long long C {static_cast<long long>(x1)*x1*bs + bs*h*h - 2*x1*h*bs +
                 static_cast<long long>(y1)*y1*as + as*k*k - 2*y1*k*as - as*bs};

    long long discriminant {B*B - 4*A*C};
    if (discriminant < 0) return false;

    double t1 {(-B + sqrt(discriminant))/(2.0*A)};
    double t2 {(-B - sqrt(discriminant))/(2.0*A)};

    return (t1>=0.0 && t1<=1.0) || (t2>=0.0 && t2<=1.0);
}

bool Ellipse::intersects(const QRect& rect) {
    if (!boundingBox().intersects(rect)) return false;

    QPoint a {rect.topLeft()};
    QPoint b {rect.topRight()};
    QPoint c {rect.bottomRight()};
    QPoint d {rect.bottomLeft()};
    return onEllipse({a, b}) || onEllipse({b, c}) || onEllipse({c, d}) || onEllipse({d, a});
};

#include "ellipse.h"

Ellipse::Ellipse() {
}

void Ellipse::m_draw(QPainter &painter, const QPointF &offset) const {
    painter.drawEllipse(QRectF(start() - offset, end() - offset));
}

bool Ellipse::onEllipse(QLineF line) const {
    int sw{m_boundingBoxPadding + getProperty(ItemProperty::StrokeWidth).value().toInt()};
    double X{m_boundingBox.x() + sw}, Y{m_boundingBox.y() + sw};
    double W{m_boundingBox.width() - 2 * sw}, H{m_boundingBox.height() - 2 * sw};

    double h{X + W / 2}, k{Y + H / 2};
    double a{W / 2}, b{H / 2};
    double x1{line.x1()}, y1{line.y1()};
    double x2{line.x2()}, y2{line.y2()};

    double p{x2 - x1}, q{y2 - y1};

    double as{a * a};
    double bs{b * b};
    double ps{p * p};
    double qs{q * q};

    double A{ps * bs + qs * as};
    double B{2 * (x1 * p * bs - p * h * bs + y1 * q * as - q * k * as)};
    double C{static_cast<long long>(x1) * x1 * bs + bs * h * h - 2 * x1 * h * bs +
             static_cast<long long>(y1) * y1 * as + as * k * k - 2 * y1 * k * as - as * bs};

    double discriminant{B * B - 4 * A * C};
    if (discriminant < 0)
        return false;

    double t1{(-B + sqrt(discriminant)) / (2.0 * A)};
    double t2{(-B - sqrt(discriminant)) / (2.0 * A)};

    return (t1 >= 0.0 && t1 <= 1.0) || (t2 >= 0.0 && t2 <= 1.0);
}

bool Ellipse::intersects(const QRectF &rect) {
    if (!boundingBox().intersects(rect))
        return false;

    QPointF a{rect.topLeft()};
    QPointF b{rect.topRight()};
    QPointF c{rect.bottomRight()};
    QPointF d{rect.bottomLeft()};
    return onEllipse({a, b}) || onEllipse({b, c}) || onEllipse({c, d}) || onEllipse({d, a});
};

bool Ellipse::intersects(const QLineF &line) {
    return onEllipse(line);
}

Item::Type Ellipse::type() const {
    return Item::Ellipse;
}

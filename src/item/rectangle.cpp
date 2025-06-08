#include "rectangle.h"

#include "../common/utils.h"
#include <QDebug>

Rectangle::Rectangle() {
}

void Rectangle::m_draw(QPainter& painter, const QPointF& offset) const {
    painter.drawRect(QRectF(start() - offset, end() - offset));
}

bool Rectangle::intersects(const QRectF& rect) {
    if (!boundingBox().intersects(rect)) return false;

    QRectF box{start(), end()};
    QPointF p{box.topLeft()};
    QPointF q{box.topRight()};
    QPointF r{box.bottomRight()};
    QPointF s{box.bottomLeft()};

    QPointF a{rect.topLeft()};
    QPointF b{rect.topRight()};
    QPointF c{rect.bottomRight()};
    QPointF d{rect.bottomLeft()};

    return (Common::intersects(QLineF{p, q}, QLineF{a, b}) ||
            Common::intersects(QLineF{p, q}, QLineF{b, c}) ||
            Common::intersects(QLineF{p, q}, QLineF{c, d}) ||
            Common::intersects(QLineF{p, q}, QLineF{d, a}) ||
            Common::intersects(QLineF{q, r}, QLineF{a, b}) ||
            Common::intersects(QLineF{q, r}, QLineF{b, c}) ||
            Common::intersects(QLineF{q, r}, QLineF{c, d}) ||
            Common::intersects(QLineF{q, r}, QLineF{d, a}) ||
            Common::intersects(QLineF{r, s}, QLineF{a, b}) ||
            Common::intersects(QLineF{r, s}, QLineF{b, c}) ||
            Common::intersects(QLineF{r, s}, QLineF{c, d}) ||
            Common::intersects(QLineF{r, s}, QLineF{d, a}) ||
            Common::intersects(QLineF{p, s}, QLineF{a, b}) ||
            Common::intersects(QLineF{p, s}, QLineF{b, c}) ||
            Common::intersects(QLineF{p, s}, QLineF{c, d}) ||
            Common::intersects(QLineF{p, s}, QLineF{d, a}));
};

bool Rectangle::intersects(const QLineF& line) {
    QRectF box{start(), end()};
    return Common::intersects(box, line);
}

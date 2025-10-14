#ifndef UTILS_H
#define UTILS_H

#include <QLineF>
#include <QRectF>

namespace Common {
inline int orientation(QPointF a, QPointF b, QPointF c) {
    QPointF ab{b.x() - a.x(), b.y() - a.y()};
    QPointF ac{c.x() - a.x(), c.y() - a.y()};

    int orient{static_cast<int>(ab.x() * ac.y() - ac.x() * ab.y())};
    return (orient == 0 ? 0 : (orient < 0 ? -1 : 1));
}

inline bool intersects(const QRectF &rectA, const QRectF &rectB) {
    return rectA.intersects(rectB);
}

inline bool intersects(const QLineF &a, const QLineF &b) {
    QPointF p{a.p1()}, q{a.p2()};
    QPointF r{b.p1()}, s{b.p2()};
    return orientation(p, q, r) != orientation(p, q, s) &&
           orientation(r, s, p) != orientation(r, s, q);
}

inline bool intersects(const QRectF &rect, const QLineF &line) {
    QLineF left{rect.topLeft(), rect.bottomLeft()};
    QLineF top{rect.topLeft(), rect.topRight()};
    QLineF right{rect.topRight(), rect.bottomRight()};
    QLineF bottom{rect.bottomRight(), rect.bottomLeft()};

    return (intersects(line, left) || intersects(line, top) || intersects(line, right) ||
            intersects(line, bottom));
}

inline bool intersects(const QRectF &rect, const QPointF &point) {
    return rect.contains(point);
}
};  // namespace Common

#endif

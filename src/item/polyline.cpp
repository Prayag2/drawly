#include "polyline.h"

Polyline::Polyline() {}

void Polyline::addPoint(const QPoint& point) {
    int x {point.x()}, y {point.y()};

    m_boundingBox = m_boundingBox.normalized();
    int bX {m_boundingBox.x()}, bY {m_boundingBox.y()};
    int bW {m_boundingBox.width()}, bH {m_boundingBox.height()};
    int mg {m_boundingBoxPadding + stroke().width()};

    QMargins adjustments {};

    if (m_points.empty()) {
        m_boundingBox.setX(x-mg);
        m_boundingBox.setY(y-mg);
        m_boundingBox.setWidth(2*mg);
        m_boundingBox.setHeight(2*mg);
    } else {
        if (x > bX+bW-mg) adjustments.setRight(x-bX-bW+mg);
        if (x < bX+mg) adjustments.setLeft(bX-x+mg);
        if (y > bY+bH-mg) adjustments.setBottom(y-bY-bH+mg);
        if (y < bY+mg) adjustments.setTop(bY-y+mg);
    }

    m_boundingBox += adjustments;
    m_points.push_back(point);
}

bool Polyline::intersects(const QRect& rect) {
    if (!boundingBox().intersects(rect)) return false;

    QPoint p {rect.topLeft()};
    QPoint q {rect.topRight()};
    QPoint r {rect.bottomRight()};
    QPoint s {rect.bottomLeft()};

    qsizetype n {m_points.size()};
    for (qsizetype idx {0}; idx < n-1; idx++) {
        QLine l {m_points[idx], m_points[idx+1]};

        if (
            Item::linesIntersect(l, {p, q}) ||
            Item::linesIntersect(l, {q, r}) ||
            Item::linesIntersect(l, {r, s}) ||
            Item::linesIntersect(l, {s, q}) ||
            rect.contains(m_points[idx]) ||
            rect.contains(m_points[idx+1])
        ) return true;
    }

    return false;
}

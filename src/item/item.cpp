#include "item.h"

// PUBLIC
Item::Item() {
    m_fill = std::make_unique<Fill>();
    m_stroke = std::make_unique<Stroke>();
    m_font = std::make_unique<Font>();
}

Stroke& Item::stroke() const {
    return *m_stroke;
}

Fill& Item::fill() const {
    return *m_fill;
}

Font& Item::font() const {
    return *m_font;
}

const QRect& Item::boundingBox() const {
    return m_boundingBox;
}

void Item::setBoundingBoxPadding(int padding) {
    m_boundingBoxPadding = padding;
}

void Item::setScale(qreal scale) {
    m_scale = scale;
}

int Item::orientation(QPoint a, QPoint b, QPoint c) {
    QPoint ab {b.x()-a.x(), b.y()-a.y()};
    QPoint ac {c.x()-a.x(), c.y()-a.y()};
    int orient {ab.x()*ac.y()-ac.x()*ab.y()};
    return (orient == 0 ? 0 : (orient < 0 ? -1 : 1));
}

bool Item::linesIntersect(QLine a, QLine b) {
    QPoint p {a.p1()}, q {a.p2()};
    QPoint r {b.p1()}, s {b.p2()};
    return orientation(p, q, r) != orientation(p, q, s) && orientation(r, s, p) != orientation(r, s, q);
}

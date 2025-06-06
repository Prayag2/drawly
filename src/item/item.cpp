#include "item.h"

// PUBLIC
Item::Item() {}

Item::~Item() {
    // TODO: Implement a basic logging system
    qDebug() << "Item deleted: " << m_boundingBox;
}

const QRectF& Item::boundingBox() const {
    return m_boundingBox;
}

void Item::setBoundingBoxPadding(int padding) {
    m_boundingBoxPadding = padding;
}

void Item::setScale(qreal scale) {
    m_scale = scale;
}

int Item::orientation(QPointF a, QPointF b, QPointF c) {
    QPointF ab{b.x() - a.x(), b.y() - a.y()};
    QPointF ac{c.x() - a.x(), c.y() - a.y()};

    int orient{static_cast<int>(ab.x() * ac.y() - ac.x() * ab.y())};
    return (orient == 0 ? 0 : (orient < 0 ? -1 : 1));
}

bool Item::linesIntersect(QLineF a, QLineF b) {
    QPointF p{a.p1()}, q{a.p2()};
    QPointF r{b.p1()}, s{b.p2()};
    return orientation(p, q, r) != orientation(p, q, s) &&
           orientation(r, s, p) != orientation(r, s, q);
}

ItemProperty& Item::getProperty(const ItemPropertyType propertyType) {
    if (m_properties.find(propertyType) == m_properties.end()) {
        throw std::logic_error("Item does not support this property.");
    }
    return m_properties.at(propertyType);
}

const ItemProperty& Item::getProperty(const ItemPropertyType propertyType) const {
    // This will call the non-const version of this method.
    // I'm doing this to avoid code duplication.
    return const_cast<Item*>(this)->getProperty(propertyType);
}

#include "item.h"

// PUBLIC
Item::Item() {
}

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

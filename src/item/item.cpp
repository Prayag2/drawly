#include "item.h"

// PUBLIC
Item::Item() {}

Item::~Item() {
    qDebug() << "Item deleted: " << m_boundingBox;
}

const QRectF& Item::boundingBox() const {
    return m_boundingBox;
}

void Item::setBoundingBoxPadding(int padding) {
    m_boundingBoxPadding = padding;
}

ItemProperty& Item::getProperty(const ItemProperty::Type propertyType) {
    if (m_properties.find(propertyType) == m_properties.end()) {
        throw std::logic_error("Item does not support this property.");
    }
    return m_properties.at(propertyType);
}

const ItemProperty& Item::getProperty(const ItemProperty::Type propertyType) const {
    // This will call the non-const version of this method.
    // I'm doing this to avoid code duplication.
    return const_cast<Item*>(this)->getProperty(propertyType);
}

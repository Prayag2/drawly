#include "item.h"
#include <algorithm>

// PUBLIC
Item::Item() {
}

Item::~Item() {
    qDebug() << "Item deleted: " << m_boundingBox;
}

const QRectF Item::boundingBox() const {
    int mg{m_boundingBoxPadding};
    return m_boundingBox.adjusted(-mg, -mg, mg, mg);
}

void Item::setBoundingBoxPadding(int padding) {
    m_boundingBoxPadding = padding;
}

const Property &Item::property(const Property::Type propertyType) const {
    if (m_properties.find(propertyType) == m_properties.end()) {
        throw std::logic_error("Item does not support this property.");
    }

    return m_properties.at(propertyType);
}

const QVector<Property::Type> Item::properties() const {
    QVector<Property::Type> result;

    std::for_each(m_properties.begin(), m_properties.end(), [&](const auto& propertyPair){
        result.push_back(propertyPair.first);
    });

    return result;
}

void Item::setProperty(const Property::Type propertyType, Property newObj) {
    if (m_properties.find(propertyType) != m_properties.end()) {
        m_properties[propertyType] = newObj;
    }

    updateAfterProperty();
}

void Item::updateAfterProperty() {}

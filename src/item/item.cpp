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

const QVector<Property::Type> Item::propertyTypes() const {
    QVector<Property::Type> result;

    for (auto& [type, _] : m_properties) {
        result.push_back(type);
    }

    return result;
}

const QVector<Property> Item::properties() const {
    QVector<Property> result;

    for (auto& [_, property] : m_properties) {
        result.push_back(property);
    }

    return result;
}

void Item::setProperty(const Property::Type propertyType, Property newObj) {
    if (m_properties.find(propertyType) != m_properties.end()) {
        m_properties[propertyType] = newObj;
    }

    updateAfterProperty();
}

void Item::updateAfterProperty() {}

int Item::boundingBoxPadding() const {
    return m_boundingBoxPadding;
}

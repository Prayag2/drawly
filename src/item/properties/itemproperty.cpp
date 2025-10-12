#include "itemproperty.h"

ItemProperty::ItemProperty() {
}

ItemProperty::ItemProperty(QVariant value) : m_value{value} {}

void ItemProperty::setValue(QVariant value) {
    m_value = value;
}

const QVariant& ItemProperty::value() const {
    return m_value;
}

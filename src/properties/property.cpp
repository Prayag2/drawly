#include "property.h"

Property::Type Property::type() const {
    return m_type;
}

const QVariant Property::variant() const {
    return m_value;
}

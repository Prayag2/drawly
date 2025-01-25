#include "itemproperty.h"

ItemProperty::ItemProperty() {}

bool ItemProperty::active() const {
    return m_isActive;
}

void ItemProperty::activate() {
    m_isActive = true;
}

void ItemProperty::deactivate() {
    m_isActive = false;
}

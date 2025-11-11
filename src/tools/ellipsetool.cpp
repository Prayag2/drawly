#include "ellipsetool.h"

#include "../item/factory/ellipsefactory.h"

EllipseTool::EllipseTool() {
    m_itemFactory = std::make_unique<EllipseFactory>();
}

QString EllipseTool::iconAlt() const {
    return "󰺡";
}

#include "ellipsetool.h"

#include "../item/factory/ellipsefactory.h"

EllipseTool::EllipseTool(const PropertyManager& propertyManager)
    : PolygonDrawingTool(propertyManager) {
    m_itemFactory = std::make_unique<EllipseFactory>();
}

QString EllipseTool::iconAlt() const {
    return "󰺡";
}

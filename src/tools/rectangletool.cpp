#include "rectangletool.h"

#include "../item/factory/rectanglefactory.h"

RectangleTool::RectangleTool(const PropertyManager& propertyManager)
    : PolygonDrawingTool(propertyManager) {
    m_itemFactory = std::make_unique<RectangleFactory>();
}

QString RectangleTool::iconAlt() const {
    return "󰹟";
}

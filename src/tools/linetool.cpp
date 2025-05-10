#include "linetool.h"

#include "../item/factory/linefactory.h"

LineTool::LineTool(const PropertyManager& propertyManager) : PolygonDrawingTool(propertyManager) {
    m_itemFactory = std::make_unique<LineFactory>();
}

QString LineTool::iconAlt() const {
    return "⟋";
}

#include "arrowtool.h"

#include "../item/factory/arrowfactory.h"

ArrowTool::ArrowTool(const PropertyManager &propertyManager) : PolygonDrawingTool(propertyManager) {
    m_itemFactory = std::make_unique<ArrowFactory>();
}

QString ArrowTool::iconAlt() const {
    return "↗";
}

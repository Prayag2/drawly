#include "rectangletool.h"

#include "../item/factory/rectanglefactory.h"

RectangleTool::RectangleTool() {
    m_itemFactory = std::make_unique<RectangleFactory>();
}

QString RectangleTool::iconAlt() const {
    return "󰹟";
}

#include "arrowtool.h"

#include "../item/factory/arrowfactory.h"

ArrowTool::ArrowTool() {
    m_itemFactory = std::make_unique<ArrowFactory>();
}

QString ArrowTool::iconAlt() const {
    return "↗";
}

#include "linetool.h"
#include "../item/factory/linefactory.h"

LineTool::LineTool() {
    m_itemFactory = std::make_unique<LineFactory>();
}

QString LineTool::iconAlt() const {
    return "⟋";
}

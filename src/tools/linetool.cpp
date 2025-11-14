#include "linetool.h"

#include "../item/factory/linefactory.h"

LineTool::LineTool() {
    m_itemFactory = std::make_unique<LineFactory>();
}

QString LineTool::tooltip() const {
    return "Line Tool";
}

IconManager::Icon LineTool::icon() const {
    return IconManager::TOOL_LINE;
}

#include "arrowtool.h"

#include "../item/factory/arrowfactory.h"

ArrowTool::ArrowTool() {
    m_itemFactory = std::make_unique<ArrowFactory>();
}

QString ArrowTool::tooltip() const {
    return "Arrow Tool";
}

IconManager::Icon ArrowTool::icon() const {
    return IconManager::TOOL_ARROW;
}

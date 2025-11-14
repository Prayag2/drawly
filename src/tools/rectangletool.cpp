#include "rectangletool.h"

#include "../item/factory/rectanglefactory.h"

RectangleTool::RectangleTool() {
    m_itemFactory = std::make_unique<RectangleFactory>();
}

QString RectangleTool::tooltip() const {
    return "Rectangle Tool";
}

IconManager::Icon RectangleTool::icon() const {
    return IconManager::TOOL_RECTANGLE;
}

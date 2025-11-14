#include "ellipsetool.h"

#include "../item/factory/ellipsefactory.h"

EllipseTool::EllipseTool() {
    m_itemFactory = std::make_unique<EllipseFactory>();
}

QString EllipseTool::tooltip() const {
    return "Ellipse Tool";
}

IconManager::Icon EllipseTool::icon() const {
    return IconManager::TOOL_ELLIPSE;
}

#include "propertymanager.h"

#include "toolstrokecolor.h"
#include "toolstrokewidth.h"

PropertyManager::PropertyManager(QWidget* parent) : QObject{parent} {
    m_properties[ToolPropertyType::StrokeWidth] = std::make_shared<ToolStrokeWidth>(parent);
    m_properties[ToolPropertyType::StrokeColor] = std::make_shared<ToolStrokeColor>(parent);
}

std::shared_ptr<ToolProperty> PropertyManager::getToolProperty(
    const ToolPropertyType toolType) const {
    if (m_properties.find(toolType) == m_properties.end()) {
        throw std::logic_error("A valid mapping for the given ToolPropertyType does not exist.");
    }
    return m_properties.at(toolType);
}

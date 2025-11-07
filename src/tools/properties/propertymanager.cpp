#include "propertymanager.h"

#include "toolerasersize.h"
#include "toolstrokecolor.h"
#include "toolstrokewidth.h"
#include "toolfontsize.h"

PropertyManager::PropertyManager(QWidget *parent) : QObject{parent} {
    m_properties[ToolProperty::StrokeWidth] = std::make_shared<ToolStrokeWidth>(parent);
    m_properties[ToolProperty::StrokeColor] = std::make_shared<ToolStrokeColor>(parent);
    m_properties[ToolProperty::EraserSize] = std::make_shared<ToolEraserSize>(parent);
    m_properties[ToolProperty::FontSize] = std::make_shared<ToolFontSize>(parent);
}

std::shared_ptr<ToolProperty> PropertyManager::get(const ToolProperty::Type toolType) const {
    if (m_properties.find(toolType) == m_properties.end()) {
        throw std::logic_error("A valid mapping for the given ToolProperty::Type does not exist.");
    }
    return m_properties.at(toolType);
}

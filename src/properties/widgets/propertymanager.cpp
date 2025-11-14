#include "propertymanager.h"

#include "erasersizewidget.h"
#include "fontsizewidget.h"
#include "strokecolorwidget.h"
#include "strokewidthwidget.h"

PropertyManager::PropertyManager(QWidget *parent) : QObject{parent} {
    m_widgets[Property::StrokeWidth] = new StrokeWidthWidget(parent);
    m_widgets[Property::StrokeColor] = new StrokeColorWidget(parent);
    m_widgets[Property::EraserSize] = new EraserSizeWidget(parent);
    m_widgets[Property::FontSize] = new FontSizeWidget(parent);

    for (const auto &[_, widget] : m_widgets) {
        QObject::connect(widget, &PropertyWidget::changed, this, &PropertyManager::propertyUpdated);
    }
}

const PropertyWidget &PropertyManager::widget(const Property::Type type) const {
    if (m_widgets.find(type) == m_widgets.end()) {
        throw std::logic_error("A valid widget for the given Property::Type does not exist.");
    }

    return *m_widgets.at(type);
}

Property PropertyManager::value(const Property::Type type) const {
    return widget(type).value();
}

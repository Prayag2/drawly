#include "updatepropertycommand.h"
#include "../item/item.h"
#include "../properties/property.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/spatialcontext.h"
#include "../data-structures/cachegrid.h"
#include <QRectF>

UpdatePropertyCommand::UpdatePropertyCommand(QVector<std::shared_ptr<Item>> items, Property newProperty)
    : ItemCommand{items}, m_newProperty{newProperty} {}

void UpdatePropertyCommand::execute(ApplicationContext *context) {
    Property::Type type{m_newProperty.type()};

    QRectF dirtyRegion{};
    for (auto& item : m_items) {
        try {
            m_properties[item] = item->property(type); 
            item->setProperty(type, m_newProperty);
            dirtyRegion |= item->boundingBox();
        } catch (const std::logic_error& e) {
            // Ignore if not found
        }
    }

    QRect gridDirtyRegion{context->spatialContext().coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->spatialContext().cacheGrid().markDirty(gridDirtyRegion);
};

void UpdatePropertyCommand::undo(ApplicationContext *context) {
    Property::Type type{m_newProperty.type()};

    QRectF dirtyRegion{};
    for (auto& item : m_items) {
        try {
            item->setProperty(type, m_properties[item]);
            dirtyRegion |= item->boundingBox();
        } catch (const std::logic_error& e) {
            // Ignore if not found
        }
    }

    QRect gridDirtyRegion{context->spatialContext().coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->spatialContext().cacheGrid().markDirty(gridDirtyRegion);
};

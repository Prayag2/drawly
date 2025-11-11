#include "selectioncontext.h"

#include "../item/item.h"
#include "applicationcontext.h"
#include "renderingcontext.h"
#include "coordinatetransformer.h"
#include "spatialcontext.h"
#include "../data-structures/cachegrid.h"
#include "../properties/property.h"
#include <stdexcept>

SelectionContext::SelectionContext(ApplicationContext *context)
    : QObject{context},
      m_applicationContext{context} {
}

SelectionContext::~SelectionContext() {
    qDebug() << "Object deleted: SelectionContext";
}

std::unordered_set<std::shared_ptr<Item>> &SelectionContext::selectedItems() {
    return m_selectedItems;
}

QRectF SelectionContext::selectionBox() const {
    QRectF selectionBox;
    for (auto item : m_selectedItems) {
        selectionBox |= item->boundingBox();
    }
    return selectionBox;
}

// PUBLIC SLOTS
void SelectionContext::updatePropertyOfSelectedItems(Property property) {
    QRectF dirtyRegion{};
    for (auto& item : m_selectedItems) {
        try {
            item->setProperty(property.type(), property);
            dirtyRegion |= item->boundingBox();
        } catch (const std::logic_error& e) {
            // Ignore if not found
        }
    }

    QRect gridDirtyRegion{m_applicationContext->spatialContext().coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    m_applicationContext->spatialContext().cacheGrid().markDirty(gridDirtyRegion);
    m_applicationContext->renderingContext().markForRender();
    m_applicationContext->renderingContext().markForUpdate();
}

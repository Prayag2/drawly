#include "removeitemcommand.h"
#include "../common/constants.h"
#include "../context/applicationcontext.h"
#include "../context/spatialcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"
#include "../item/item.h"

RemoveItemCommand::RemoveItemCommand(std::shared_ptr<Item> item)
    : ItemCommand{item} {}

void RemoveItemCommand::execute(ApplicationContext* context) {
    auto& transformer{context->spatialContext().coordinateTransformer()};
    QRect dirtyRegion{transformer.worldToGrid(m_item->boundingBox()).toRect()};

    context->spatialContext().quadtree().deleteItem(m_item);
    context->spatialContext().cacheGrid().markDirty(dirtyRegion);
}

void RemoveItemCommand::undo(ApplicationContext* context) {
    auto& transformer{context->spatialContext().coordinateTransformer()};
    QRect dirtyRegion{transformer.worldToGrid(m_item->boundingBox()).toRect()};

    m_item->getProperty(ItemPropertyType::Opacity).setValue(Common::maxItemOpacity);
    context->spatialContext().quadtree().insertItem(m_item);
    context->spatialContext().cacheGrid().markDirty(dirtyRegion);
}

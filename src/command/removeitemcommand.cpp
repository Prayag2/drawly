#include "removeitemcommand.h"

#include "../common/constants.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/spatialcontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../item/item.h"

RemoveItemCommand::RemoveItemCommand(QVector<std::shared_ptr<Item>> items) : ItemCommand{items} {
}

void RemoveItemCommand::execute(ApplicationContext *context) {
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &quadtree{context->spatialContext().quadtree()};
    auto &cacheGrid{context->spatialContext().cacheGrid()};

    for (auto &item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};

        quadtree.deleteItem(item);
        cacheGrid.markDirty(dirtyRegion);
    }
}

void RemoveItemCommand::undo(ApplicationContext *context) {
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &quadtree{context->spatialContext().quadtree()};
    auto &cacheGrid{context->spatialContext().cacheGrid()};

    for (auto &item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};

        quadtree.insertItem(item);
        cacheGrid.markDirty(dirtyRegion);
    }
}

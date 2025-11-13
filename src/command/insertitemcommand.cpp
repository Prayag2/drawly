#include "insertitemcommand.h"

#include "../context/applicationcontext.h"
#include "../context/selectioncontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/spatialcontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"

InsertItemCommand::InsertItemCommand(QVector<std::shared_ptr<Item>> items) : ItemCommand{items} {
}

InsertItemCommand::~InsertItemCommand() {
}

void InsertItemCommand::execute(ApplicationContext *context) {
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &quadtree{context->spatialContext().quadtree()};
    auto &cacheGrid{context->spatialContext().cacheGrid()};

    for (auto &item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};
        quadtree.insertItem(item);
        cacheGrid.markDirty(dirtyRegion);
    }
}

void InsertItemCommand::undo(ApplicationContext *context) {
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &quadtree{context->spatialContext().quadtree()};
    auto &cacheGrid{context->spatialContext().cacheGrid()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    for (auto &item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};

        selectedItems.erase(item);
        quadtree.deleteItem(item);
        cacheGrid.markDirty(dirtyRegion);
    }
}

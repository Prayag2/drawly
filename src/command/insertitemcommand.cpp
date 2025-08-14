#include "insertitemcommand.h"
#include "../context/applicationcontext.h"
#include "../context/spatialcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"

InsertItemCommand::InsertItemCommand(QVector<std::shared_ptr<Item>> items)
    : ItemCommand{items} {}

InsertItemCommand::~InsertItemCommand() {}

void InsertItemCommand::execute(ApplicationContext* context) {
    auto& transformer{context->spatialContext().coordinateTransformer()};
    auto& quadtree{context->spatialContext().quadtree()};
    auto& cacheGrid{context->spatialContext().cacheGrid()};

    for (auto& item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};
        quadtree.insertItem(item);
        cacheGrid.markDirty(dirtyRegion);
    }
}

void InsertItemCommand::undo(ApplicationContext* context) {
    auto& transformer{context->spatialContext().coordinateTransformer()};
    auto& quadtree{context->spatialContext().quadtree()};
    auto& cacheGrid{context->spatialContext().cacheGrid()};

    for (auto& item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};
        quadtree.deleteItem(item);
        cacheGrid.markDirty(dirtyRegion);
    }
}

#include "insertitemcommand.h"
#include "../context/applicationcontext.h"
#include "../context/spatialcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"

InsertItemCommand::InsertItemCommand(std::shared_ptr<Item> item)
    : ItemCommand{item} {}

InsertItemCommand::~InsertItemCommand() {}

void InsertItemCommand::execute(ApplicationContext* context) {
    auto& transformer{context->spatialContext().coordinateTransformer()};
    QRect dirtyRegion{transformer.worldToGrid(m_item->boundingBox()).toRect()};

    context->spatialContext().quadtree().insertItem(m_item);
    context->spatialContext().cacheGrid().markDirty(dirtyRegion);
}

void InsertItemCommand::undo(ApplicationContext* context) {
    auto& transformer{context->spatialContext().coordinateTransformer()};
    QRect dirtyRegion{transformer.worldToGrid(m_item->boundingBox()).toRect()};

    context->spatialContext().quadtree().deleteItem(m_item);
    context->spatialContext().cacheGrid().markDirty(dirtyRegion);
}

#include "moveitemcommand.h"

#include "../common/constants.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/spatialcontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../item/item.h"

MoveItemCommand::MoveItemCommand(QVector<std::shared_ptr<Item>> items, QPointF delta)
    : ItemCommand{items},
      m_delta{delta} {
}

void MoveItemCommand::execute(ApplicationContext *context) {
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &quadtree{context->spatialContext().quadtree()};
    auto &cacheGrid{context->spatialContext().cacheGrid()};

    for (auto &item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};

        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        item->translate(m_delta);
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

void MoveItemCommand::undo(ApplicationContext *context) {
    auto &transformer{context->spatialContext().coordinateTransformer()};
    auto &quadtree{context->spatialContext().quadtree()};
    auto &cacheGrid{context->spatialContext().cacheGrid()};

    for (auto &item : m_items) {
        QRect dirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};

        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        item->translate(-m_delta);
        cacheGrid.markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
    }
}

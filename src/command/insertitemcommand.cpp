/*
* Drawy - A simple brainstorming tool with an infinite canvas
* Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "insertitemcommand.h"

#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/selectioncontext.h"
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

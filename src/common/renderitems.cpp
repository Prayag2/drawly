#include "renderitems.h"

#include "../canvas/canvas.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../item/item.h"
#include <QPointF>
#include <QRectF>
#include <memory>

// TODO: Optimize this to improve performance
void Common::renderCanvas(ApplicationContext* context) {
    auto& transformer{context->coordinateTransformer()};

    context->canvas().canvas()->fill(context->canvas().bg());

    QPointF gridOffset{transformer.worldToGrid(context->offsetPos())};
    QRectF gridViewport(gridOffset, context->canvas().dimensions());

    QVector<std::shared_ptr<CacheCell>> visibleCells{
        context->cacheGrid().queryCells(gridViewport.toRect())};

    QPainter& canvasPainter{context->canvasPainter()};

    for (auto cell : visibleCells) {
        // QPen pen; pen.setColor(Qt::white); painter.setPen(pen);
        // painter.drawRect(cell->rect().translated(-viewOffset.toPoint()));

        if (cell->dirty()) {
            cell->image().fill(Qt::transparent);
            cell->setDirty(false);

            QVector<std::shared_ptr<Item>> intersectingItems{context->quadtree().queryItems(
                transformer.gridToWorld(cell->rect()), [](auto a, auto b) { return true; })};

            if (intersectingItems.empty()) continue;

            cell->painter().resetTransform();
            cell->painter().scale(context->zoomFactor(), context->zoomFactor());

            QPointF topLeftPoint{transformer.gridToWorld(cell->rect().topLeft())};

            for (auto intersectingItem : intersectingItems) {
                intersectingItem->draw(cell->painter(), topLeftPoint);
            }
        }

        canvasPainter.drawImage(cell->rect().translated(-gridOffset.toPoint()), cell->image());
    }

    QRectF selectionBox{};
    std::unordered_set<std::shared_ptr<Item>>& selectedItems{context->selectedItems()};

    if (selectedItems.empty()) return;

    // render a box around selected items
    canvasPainter.save();
    QPen pen{QColor{67, 135, 244}};
    canvasPainter.setPen(pen);

    for (auto item : selectedItems) {
        QRectF curBox{transformer.worldToView(item->boundingBox()).normalized()};
        canvasPainter.drawRect(curBox);
        selectionBox |= curBox;
    }

    pen.setStyle(Qt::DashLine);
    canvasPainter.setPen(pen);
    canvasPainter.drawRect(selectionBox);
    canvasPainter.restore();

    /* TODO: Implement resizing
    overlayPainter.save();
    pen.setStyle(Qt::SolidLine); overlayPainter.setPen(pen);
    overlayPainter.setBrush(Qt::white);

    QRectF handle{-5/context->zoomFactor(), -5/context->zoomFactor(), 10/context->zoomFactor(),
    10/context->zoomFactor()}; handle.translate(-context->offsetPos());
    overlayPainter.drawEllipse(handle.translated(selectionBox.topLeft()));
    overlayPainter.drawEllipse(handle.translated(selectionBox.topRight()));
    overlayPainter.drawEllipse(handle.translated(selectionBox.bottomRight()));
    overlayPainter.drawEllipse(handle.translated(selectionBox.bottomLeft()));

    overlayPainter.restore();
    */
}

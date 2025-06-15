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
void Common::renderItems(ApplicationContext* context) {
    auto& transformer{context->coordinateTransformer()};

    QPointF viewOffset{transformer.toView(context->offsetPos())};
    QRectF viewViewport(viewOffset, context->canvas().dimensions());

    context->canvas().setBg(context->canvas().bg());
    QVector<std::shared_ptr<CacheCell>> visibleCells{
        context->cacheGrid().queryCells(viewViewport.toRect())};

    QPainter& painter{context->canvasPainter()};
    for (auto cell : visibleCells) {
        // QPen pen; pen.setColor(Qt::white); painter.setPen(pen);
        // painter.drawRect(cell->rect().translated(-viewOffset.toPoint()));

        if (cell->dirty()) {
            cell->image().fill(Qt::transparent);
            cell->setDirty(false);

            QVector<std::shared_ptr<Item>> intersectingItems{
                context->quadtree().queryItems(transformer.toWorld(cell->rect()), [](auto a, auto b){
                    return true;
                })};

            if (intersectingItems.empty()) continue;

            cell->painter().resetTransform();
            cell->painter().scale(context->zoomFactor(), context->zoomFactor());

            QPointF topLeftPoint{transformer.toWorld(cell->rect().topLeft())};

            for (auto intersectingItem : intersectingItems) {
                intersectingItem->draw(cell->painter(), topLeftPoint);
            }
        }

        painter.drawImage(cell->rect().translated(-viewOffset.toPoint()), cell->image());
    }

    QRectF selectionBox{};
    std::unordered_set<std::shared_ptr<Item>>& selectedItems{context->selectedItems()};

    if (selectedItems.empty()) return;

    // render a box around selected items
    QPainter& overlayPainter{context->overlayPainter()};
    overlayPainter.save();
    QPen pen{QColor{67, 135, 244}}; pen.setWidth(1/context->zoomFactor());
    overlayPainter.setPen(pen);

    for (auto item : selectedItems) {
        overlayPainter.drawRect(item->boundingBox().translated(-context->offsetPos()));
        selectionBox |= item->boundingBox().normalized();
    }

    pen.setStyle(Qt::DotLine); overlayPainter.setPen(pen);
    overlayPainter.drawRect(selectionBox.translated(-context->offsetPos()));
    overlayPainter.restore();

    /* TODO: Implement resizing
    // TODO: Remove magic number
    overlayPainter.save();
    pen.setStyle(Qt::SolidLine); overlayPainter.setPen(pen);
    overlayPainter.setBrush(Qt::white);

    QRectF handle{-5/context->zoomFactor(), -5/context->zoomFactor(), 10/context->zoomFactor(), 10/context->zoomFactor()}; handle.translate(-context->offsetPos());
    overlayPainter.drawEllipse(handle.translated(selectionBox.topLeft()));
    overlayPainter.drawEllipse(handle.translated(selectionBox.topRight()));
    overlayPainter.drawEllipse(handle.translated(selectionBox.bottomRight()));
    overlayPainter.drawEllipse(handle.translated(selectionBox.bottomLeft()));

    overlayPainter.restore();
    */
}

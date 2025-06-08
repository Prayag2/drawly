#include "renderitems.h"

#include "../canvas/canvas.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../item/item.h"
#include <QPointF>
#include <QRectF>

// TODO: Optimize this to improve performance
void Common::renderItems(ApplicationContext* context) {
    auto& transformer{context->coordinateTransformer()};

    QPointF viewOffset{transformer.toView(context->offsetPos())};
    QRectF viewViewport(viewOffset, context->canvas().dimensions());

    context->canvas().setBg(context->canvas().bg());
    QVector<std::shared_ptr<CacheCell>> visibleCells{
        context->cacheGrid().queryCells(viewViewport.toRect())};

    QPainter& painter{context->canvasPainter()};
    QPainter& overlayPainter{context->overlayPainter()};
    for (auto cell : visibleCells) {
        // QPen pen; pen.setColor(Qt::white); painter.setPen(pen);
        // painter.drawRect(cell->rect().translated(-viewOffset.toPoint()));

        if (cell->dirty()) {
            cell->image().fill(Qt::transparent);
            cell->setDirty(false);

            QVector<std::shared_ptr<Item>> intersectingItems{
                context->quadtree().queryItems(transformer.toWorld(cell->rect()), true)};

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
}

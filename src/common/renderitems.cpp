#include "renderitems.h"

#include "../canvas/canvas.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/renderingcontext.h"
#include "../context/selectioncontext.h"
#include "../context/spatialcontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../item/item.h"
#include "constants.h"
#include <QPointF>
#include <QRectF>
#include <memory>

// TODO: Refactor this
void Common::renderCanvas(ApplicationContext *context) {
    CoordinateTransformer &transformer{context->spatialContext().coordinateTransformer()};
    Canvas &canvas{context->renderingContext().canvas()};
    QPointF offsetPos{context->spatialContext().offsetPos()};

    canvas.canvas()->fill(canvas.bg());

    QPointF gridOffset{transformer.worldToGrid(offsetPos)};
    QRectF gridViewport(gridOffset, transformer.viewToGrid(canvas.dimensions()));

    QVector<std::shared_ptr<CacheCell>> visibleCells{
        context->spatialContext().cacheGrid().queryCells(transformer.round(gridViewport))};

    QPainter &canvasPainter{context->renderingContext().canvasPainter()};

    for (auto cell : visibleCells) {
        // canvasPainter.save();
        // QPen pen; pen.setColor(Qt::white); canvasPainter.setPen(pen);
        // canvasPainter.drawRect(transformer.gridToView(cell->rect()));
        // canvasPainter.restore();

        if (cell->dirty()) {
            cell->image().fill(Qt::transparent);
            cell->setDirty(false);

            QVector<std::shared_ptr<Item>> intersectingItems{
                context->spatialContext().quadtree().queryItems(
                    transformer.gridToWorld(cell->rect()),
                    [](auto a, auto b) { return true; })};

            if (intersectingItems.empty())
                continue;

            qreal zoomFactor{context->renderingContext().zoomFactor()};

            QPointF topLeftPoint{transformer.gridToWorld(cell->rect().topLeft().toPointF())};

            cell->painter().resetTransform();
            cell->painter().scale(zoomFactor, zoomFactor);

            for (auto intersectingItem : intersectingItems) {
                intersectingItem->draw(cell->painter(), topLeftPoint);
            }
        }

        canvasPainter.drawPixmap(transformer.round(transformer.gridToView(cell->rect())),
                                 cell->image());
    }

    QRectF selectionBox{};
    auto &selectedItems{context->selectionContext().selectedItems()};

    if (selectedItems.empty())
        return;

    // render a box around selected items
    canvasPainter.save();
    QPen pen{Common::selectionBorderColor};
    pen.setWidth(2);

    canvasPainter.setPen(pen);

    for (auto item : selectedItems) {
        QRectF curBox{transformer.worldToView(item->boundingBox()).normalized()};
        canvasPainter.drawRect(curBox);
        selectionBox |= curBox;
    }

    canvasPainter.setPen(pen);
    canvasPainter.drawRect(selectionBox);
    canvasPainter.restore();
}

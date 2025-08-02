#include "renderitems.h"

#include "../canvas/canvas.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/selectioncontext.h"
#include "../context/spatialcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../item/item.h"
#include <QPointF>
#include <QRectF>
#include <memory>

// TODO: Optimize this to improve performance
void Common::renderCanvas(ApplicationContext* context) {
    CoordinateTransformer& transformer{context->spatialContext().coordinateTransformer()};
    Canvas& canvas{context->renderingContext().canvas()};
    QPointF offsetPos{context->spatialContext().offsetPos()};

    canvas.canvas()->fill(canvas.bg());

    QPointF gridOffset{transformer.worldToGrid(offsetPos)};
    QRectF gridViewport(gridOffset, canvas.dimensions());

    QVector<std::shared_ptr<CacheCell>> visibleCells{
        context->spatialContext().cacheGrid().queryCells(gridViewport.toRect())};

    QPainter& canvasPainter{context->renderingContext().canvasPainter()};

    for (auto cell : visibleCells) {
        // QPen pen; pen.setColor(Qt::white); painter.setPen(pen);
        // painter.drawRect(cell->rect().translated(-viewOffset.toPoint()));

        if (cell->dirty()) {
            cell->image().fill(Qt::transparent);
            cell->setDirty(false);

            QVector<std::shared_ptr<Item>> intersectingItems{context->spatialContext().quadtree().queryItems(
                transformer.gridToWorld(cell->rect()), [](auto a, auto b) { return true; })};

            if (intersectingItems.empty()) continue;

            qreal zoomFactor{context->renderingContext().zoomFactor()};

            cell->painter().resetTransform();
            cell->painter().scale(zoomFactor, zoomFactor);

            QPointF topLeftPoint{transformer.gridToWorld(cell->rect().topLeft())};

            for (auto intersectingItem : intersectingItems) {
                intersectingItem->draw(cell->painter(), topLeftPoint);
            }
        }

        canvasPainter.drawImage(cell->rect().translated(-gridOffset.toPoint()), cell->image());
    }

    QRectF selectionBox{};
    auto& selectedItems{context->selectionContext().selectedItems()};

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
}

#include <QPainter>
#include "selectiontool.h"
#include "../common/renderitems.h"
#include "../canvas/canvas.h"
#include "../data-structures/cachegrid.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../event/event.h"
#include "../item/item.h"
#include "../data-structures/quadtree.h"
#include <memory>

SelectionTool::SelectionTool() {
    m_cursor = QCursor(Qt::ArrowCursor);
}

QString SelectionTool::iconAlt() const {
    return "󰆿";
}

// TODO: Implement resizing, rotation, and deleting selected items as well
void SelectionTool::mousePressed(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        auto& transformer = context->coordinateTransformer();
        QPointF worldPos{transformer.toWorld(context->event().pos()) + context->offsetPos()};

        if (context->selectionBox().contains(worldPos)) {
            m_isMoving = true;
            m_lastPos = worldPos;
        } else {
            m_isSelecting = true;
            m_startPoint = worldPos;
            context->selectedItems().clear();

            Common::renderItems(context);
            context->canvas().update();
        }
    }
};

void SelectionTool::mouseMoved(ApplicationContext* context) {
    auto& transformer = context->coordinateTransformer();
    QPointF worldPos{transformer.toWorld(context->event().pos()) + context->offsetPos()};
    std::unordered_set<std::shared_ptr<Item>>& selectedItems{context->selectedItems()};

    if (m_isSelecting) {
        QRectF selectionBox{m_startPoint, worldPos};

        QVector<std::shared_ptr<Item>> intersectingItems{
            context->quadtree().queryItems(selectionBox, [](std::shared_ptr<Item> item, const QRectF& rect){
                return rect.contains(item->boundingBox());
            })
        };

        selectedItems = std::unordered_set(intersectingItems.begin(), intersectingItems.end());

        Common::renderItems(context);

        QPainter& overlayPainter{context->overlayPainter()};
        overlayPainter.save();
        QPen pen{QColor{67, 135, 244, 200}}; overlayPainter.setPen(pen);

        QRectF selectionBoxView{selectionBox.translated(-context->offsetPos())};
        overlayPainter.drawRect(selectionBoxView);
        overlayPainter.fillRect(selectionBoxView, QColor{67, 135, 244, 50});
        overlayPainter.restore();

        context->canvas().update();
        return;
    }

    if (m_isMoving) {
        context->canvas().setCursor(Qt::ClosedHandCursor);

        for (auto item : selectedItems) {
            QRectF oldBoundingBox{item->boundingBox()};

            context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());
            item->translate(worldPos - m_lastPos);
            context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());

            context->quadtree().updateItem(item, oldBoundingBox);
        }

        m_lastPos = worldPos;
        Common::renderItems(context);
        context->canvas().update();
        return;
    }

    if (context->selectionBox().contains(worldPos)) {
        context->canvas().setCursor(Qt::OpenHandCursor);
    } else {
        context->canvas().setCursor(Qt::ArrowCursor);
    }
};

void SelectionTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isSelecting = false;
        m_isMoving = false;

        Common::renderItems(context);
        context->canvas().update();
    }
};

const bool SelectionTool::lowFpsTolerant() const {
    return true;
};

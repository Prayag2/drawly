#include "selectiontool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/item.h"
#include <QPainter>
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
        QPointF curPoint{context->event().pos()};

        if (context->selectionBox().contains(transformer.viewToWorld(curPoint))) {
            m_isMoving = true;
            m_lastPos = curPoint;
        } else {
            m_isSelecting = true;
            m_startPoint = curPoint;
            context->selectedItems().clear();

            context->markForRender();
            context->markForUpdate();
        }
    }
};

void SelectionTool::mouseMoved(ApplicationContext* context) {
    auto& transformer = context->coordinateTransformer();

    QPointF curPoint{context->event().pos()};
    QPointF worldPoint{transformer.viewToWorld(curPoint)};

    context->canvas().overlay()->fill(Qt::transparent);

    std::unordered_set<std::shared_ptr<Item>>& selectedItems{context->selectedItems()};

    if (m_isSelecting) {
        QRectF selectionBox{m_startPoint, curPoint};
        QRectF worldSelectionBox{transformer.viewToWorld(selectionBox)};

        QVector<std::shared_ptr<Item>> intersectingItems{context->quadtree().queryItems(
            worldSelectionBox, [](std::shared_ptr<Item> item, const QRectF& rect) {
                return rect.contains(item->boundingBox());
            })};

        selectedItems = std::unordered_set(intersectingItems.begin(), intersectingItems.end());

        QPainter& overlayPainter{context->overlayPainter()};
        overlayPainter.save();
        QPen pen{QColor{67, 135, 244, 200}};
        overlayPainter.setPen(pen);

        overlayPainter.drawRect(selectionBox);
        overlayPainter.fillRect(selectionBox, QColor{67, 135, 244, 50});

        overlayPainter.restore();

        context->markForUpdate();
        return;
    }

    if (m_isMoving) {
        context->canvas().setCursor(Qt::ClosedHandCursor);

        QPointF displacement{worldPoint - transformer.viewToWorld(m_lastPos)};
        for (auto item : selectedItems) {
            QRectF oldBoundingBox{item->boundingBox()};

            context->cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
            item->translate(displacement);
            context->cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

            context->quadtree().updateItem(item, oldBoundingBox);
        }

        m_lastPos = curPoint;
        context->markForRender();
        context->markForUpdate();
        return;
    }

    if (context->selectionBox().contains(worldPoint)) {
        context->canvas().setCursor(Qt::OpenHandCursor);
    } else {
        context->canvas().setCursor(Qt::ArrowCursor);
    }
};

void SelectionTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isSelecting = false;
        m_isMoving = false;

        // clearing the overlay
        context->canvas().overlay()->fill(Qt::transparent);
        context->markForRender();
        context->markForUpdate();
    }
};

ToolID SelectionTool::id() const {
    return ToolID::SelectionToolID;
};

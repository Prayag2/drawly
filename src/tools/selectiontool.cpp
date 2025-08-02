#include "selectiontool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/selectioncontext.h"
#include "../context/spatialcontext.h"
#include "../context/uicontext.h"
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
    UIContext& uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext& spatialContext{context->spatialContext()};
        CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};
        RenderingContext& renderingContext{context->renderingContext()};
        SelectionContext& selectionContext{context->selectionContext()};

        QPointF curPoint{uiContext.event().pos()};

        if (selectionContext.selectionBox().contains(transformer.viewToWorld(curPoint))) {
            m_isMoving = true;
            m_lastPos = curPoint;
        } else {
            m_isSelecting = true;
            m_startPoint = curPoint;
            selectionContext.selectedItems().clear();

            renderingContext.markForRender();
            renderingContext.markForUpdate();
        }
    }
};

void SelectionTool::mouseMoved(ApplicationContext* context) {
    SpatialContext& spatialContext{context->spatialContext()};
    CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};
    RenderingContext& renderingContext{context->renderingContext()};
    SelectionContext& selectionContext{context->selectionContext()};
    UIContext& uiContext{context->uiContext()};

    QPointF curPoint{uiContext.event().pos()};
    QPointF worldPoint{transformer.viewToWorld(curPoint)};

    renderingContext.canvas().overlay()->fill(Qt::transparent);

    std::unordered_set<std::shared_ptr<Item>>& selectedItems{selectionContext.selectedItems()};

    if (m_isSelecting) {
        QRectF selectionBox{m_startPoint, curPoint};
        QRectF worldSelectionBox{transformer.viewToWorld(selectionBox)};

        QVector<std::shared_ptr<Item>> intersectingItems{spatialContext.quadtree().queryItems(
            worldSelectionBox, [](std::shared_ptr<Item> item, const QRectF& rect) {
                return rect.contains(item->boundingBox());
            })};

        selectedItems = std::unordered_set(intersectingItems.begin(), intersectingItems.end());

        QPainter& overlayPainter{renderingContext.overlayPainter()};
        overlayPainter.save();
        QPen pen{QColor{67, 135, 244, 200}};
        overlayPainter.setPen(pen);

        overlayPainter.drawRect(selectionBox);
        overlayPainter.fillRect(selectionBox, QColor{67, 135, 244, 50});

        overlayPainter.restore();

        renderingContext.markForUpdate();
        return;
    }

    if (m_isMoving) {
        renderingContext.canvas().setCursor(Qt::ClosedHandCursor);

        QPointF displacement{worldPoint - transformer.viewToWorld(m_lastPos)};
        for (auto item : selectedItems) {
            QRectF oldBoundingBox{item->boundingBox()};

            spatialContext.cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
            item->translate(displacement);
            spatialContext.cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

            spatialContext.quadtree().updateItem(item, oldBoundingBox);
        }

        m_lastPos = curPoint;
        renderingContext.markForRender();
        renderingContext.markForUpdate();
        return;
    }

    if (selectionContext.selectionBox().contains(worldPoint)) {
        renderingContext.canvas().setCursor(Qt::OpenHandCursor);
    } else {
        renderingContext.canvas().setCursor(Qt::ArrowCursor);
    }
};

void SelectionTool::mouseReleased(ApplicationContext* context) {
    UIContext& uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext& spatialContext{context->spatialContext()};
        CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};
        RenderingContext& renderingContext{context->renderingContext()};
        SelectionContext& selectionContext{context->selectionContext()};

        m_isSelecting = false;
        m_isMoving = false;

        // clearing the overlay
        renderingContext.canvas().overlay()->fill(Qt::transparent);
        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

ToolID SelectionTool::id() const {
    return ToolID::SelectionToolID;
};

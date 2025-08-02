#include "erasertool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../common/constants.h"
#include "../common/utils.h"
#include "../context/applicationcontext.h"
#include "../context/uicontext.h"
#include "../context/spatialcontext.h"
#include "../context/selectioncontext.h"
#include "../context/renderingcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/item.h"
#include "properties/propertymanager.h"
#include "properties/toolproperty.h"
#include <QDebug>
#include <QPainter>

EraserTool::EraserTool(const PropertyManager& propertyManager) {
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties[ToolPropertyType::EraserSize] =
        (propertyManager.getToolProperty(ToolPropertyType::EraserSize));
}

QString EraserTool::iconAlt() const {
    return "";
}

void EraserTool::mousePressed(ApplicationContext* context) {
    Event& event{context->uiContext().event()};

    if (event.button() == Qt::LeftButton) {
        m_isErasing = true;
    }
};

// FIXME: messy code
void EraserTool::mouseMoved(ApplicationContext* context) {
    SpatialContext& spatialContext{context->spatialContext()};
    RenderingContext& renderingContext{context->renderingContext()};
    UIContext& uiContext{context->uiContext()};
    CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};

    QPainter& overlayPainter{renderingContext.overlayPainter()};

    // Erase previous box
    overlayPainter.save();
    overlayPainter.setCompositionMode(QPainter::CompositionMode_Source);
    overlayPainter.fillRect(m_lastRect + Drawly::cleanupMargin, Qt::transparent);

    const int eraserSide{m_properties[ToolPropertyType::EraserSize]->value().toInt()};
    const QSize eraserSize{eraserSide, eraserSide};

    // TODO: Adjustable eraser size
    double eraserCenterOffset{eraserSide / 2.0 - 1};
    QPointF eraserCenterOffsetPoint{eraserCenterOffset, eraserCenterOffset};

    QRectF curRect{uiContext.event().pos() - eraserCenterOffsetPoint, eraserSize};
    QRectF worldEraserRect{transformer.viewToWorld(curRect)};

    if (m_isErasing) {
        QVector<std::shared_ptr<Item>> toBeErased{spatialContext.quadtree().queryItems(worldEraserRect)};

        for (std::shared_ptr<Item> item : toBeErased) {
            if (m_toBeErased.count(item) > 0) continue;

            // TODO: Remove magic numbers
            item->getProperty(ItemPropertyType::StrokeColor).setValue(Drawly::erasedItemColor);

            m_toBeErased.insert(item);
            spatialContext.cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
            renderingContext.markForRender();
        }

        overlayPainter.fillRect(curRect, Drawly::eraserBackgroundColor);
    }

    renderingContext.markForUpdate();

    // Draw eraser box
    QPen pen{Drawly::eraserBorderColor, Drawly::eraserBorderWidth};
    overlayPainter.setPen(pen);
    overlayPainter.drawRect(curRect);
    overlayPainter.restore();

    renderingContext.markForUpdate();

    m_lastRect = curRect;
}

void EraserTool::mouseReleased(ApplicationContext* context) {
    UIContext& uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext& spatialContext{context->spatialContext()};
        CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};
        RenderingContext& renderingContext{context->renderingContext()};
        SelectionContext& selectionContext{context->selectionContext()};

        for (std::shared_ptr<Item> item : m_toBeErased) {
            if (selectionContext.selectedItems().count(item) > 0) {
                selectionContext.selectedItems().erase(item);
            }

            spatialContext.quadtree().deleteItem(item);
            spatialContext.cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        }

        renderingContext.markForRender();
        renderingContext.markForUpdate();

        m_toBeErased.clear();
        m_isErasing = false;
    }
}

ToolID EraserTool::id() const {
    return ToolID::EraserToolID;
}

#include "polygondrawingtool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/factory/itemfactory.h"
#include "../item/polygon.h"
#include "../item/properties/itemproperty.h"
#include "properties/propertymanager.h"
#include "properties/toolproperty.h"

PolygonDrawingTool::PolygonDrawingTool(const PropertyManager& propertyManager) {
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties[ToolPropertyType::StrokeWidth] =
        (propertyManager.getToolProperty(ToolPropertyType::StrokeWidth));
    m_properties[ToolPropertyType::StrokeColor] =
        (propertyManager.getToolProperty(ToolPropertyType::StrokeColor));
}

void PolygonDrawingTool::mousePressed(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        auto& transformer{context->coordinateTransformer()};

        curItem = std::dynamic_pointer_cast<Polygon>(m_itemFactory->create());

        curItem->getProperty(ItemPropertyType::StrokeWidth)
            .setValue(m_properties[ToolPropertyType::StrokeWidth]->value());
        curItem->getProperty(ItemPropertyType::StrokeColor)
            .setValue(m_properties[ToolPropertyType::StrokeColor]->value());

        curItem->setScale(context->canvas().scale());
        curItem->setBoundingBoxPadding(10 * context->canvas().scale());
        curItem->setStart(transformer.viewToWorld(context->event().pos()));

        QPainter& painter{context->overlayPainter()};
        painter.save();
        painter.scale(context->zoomFactor(), context->zoomFactor());

        m_isDrawing = true;
    }
};

void PolygonDrawingTool::mouseMoved(ApplicationContext* context) {
    if (m_isDrawing) {
        auto& transformer{context->coordinateTransformer()};
        QPainter& overlayPainter{context->overlayPainter()};

        curItem->erase(overlayPainter, context->offsetPos());
        curItem->setEnd(transformer.viewToWorld(context->event().pos()));
        curItem->draw(overlayPainter, context->offsetPos());

        context->markForUpdate();
    }
};

void PolygonDrawingTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton && m_isDrawing) {
        auto& transformer{context->coordinateTransformer()};

        context->quadtree().insertItem(curItem);

        QPainter& overlayPainter{context->overlayPainter()};
        context->canvas().overlay()->fill(Qt::transparent);
        overlayPainter.restore();

        context->cacheGrid().markDirty(transformer.worldToGrid(curItem->boundingBox()).toRect());

        m_isDrawing = false;

        context->markForRender();
        context->markForUpdate();
    }
};

ToolID PolygonDrawingTool::id() const {
    return ToolID::PolygonDrawingToolID;
}


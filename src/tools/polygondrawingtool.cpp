#include "polygondrawingtool.h"
#include "properties/propertymanager.h"
#include "../context/applicationcontext.h"
#include "../item/properties/itemproperty.h"
#include "../item/factory/itemfactory.h"
#include "../item/polygon.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "properties/toolproperty.h"

PolygonDrawingTool::PolygonDrawingTool(const PropertyManager& propertyManager) {
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties[ToolPropertyType::StrokeWidth] = (propertyManager.getToolProperty(ToolPropertyType::StrokeWidth));
    m_properties[ToolPropertyType::StrokeColor] = (propertyManager.getToolProperty(ToolPropertyType::StrokeColor));
}

void PolygonDrawingTool::mousePressed(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        curItem = std::dynamic_pointer_cast<Polygon>(m_itemFactory->create());

        // properties
        curItem->getProperty(ItemPropertyType::StrokeWidth).setValue(m_properties[ToolPropertyType::StrokeWidth]->value());
        curItem->getProperty(ItemPropertyType::StrokeColor).setValue(m_properties[ToolPropertyType::StrokeColor]->value());

        curItem->setScale(context->canvas().scale());
        curItem->setBoundingBoxPadding(10 * context->canvas().scale());
        curItem->setStart(context->event().pos() - context->offsetPos());

        m_isDrawing = true;
    }
};

void PolygonDrawingTool::mouseMoved(ApplicationContext *context) {
    if (m_isDrawing) {
        QPainter& overlayPainter {context->overlayPainter()};

        curItem->erase(overlayPainter, context->offsetPos());
        curItem->setEnd(context->event().pos() - context->offsetPos());
        curItem->draw(overlayPainter, context->offsetPos());

        context->canvas().update();
    }
};

void PolygonDrawingTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton && m_isDrawing) {
        QPainter& overlayPainter {context->overlayPainter()};
        QPainter& canvasPainter {context->canvasPainter()};

        curItem->erase(overlayPainter, context->offsetPos());
        curItem->draw(canvasPainter, context->offsetPos());

        context->quadtree().insertItem(curItem);

        m_isDrawing = false;
        context->canvas().update();
    }
};

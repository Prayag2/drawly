#include "freeformtool.h"
#include "../item/factory/freeformfactory.h"
#include "../context/applicationcontext.h"
#include "../item/freeform.h"
#include "../event/event.h"
#include "../canvas/canvas.h"
#include "../item/item.h"
#include "../data-structures/quadtree.h"
#include "properties/propertymanager.h"
#include "properties/toolproperty.h"

FreeformTool::FreeformTool(const PropertyManager& propertyManager) {
    m_itemFactory = std::make_unique<FreeformFactory>();
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties[ToolPropertyType::StrokeWidth] = (propertyManager.getToolProperty(ToolPropertyType::StrokeWidth));
    m_properties[ToolPropertyType::StrokeColor] = (propertyManager.getToolProperty(ToolPropertyType::StrokeColor));
}

QString FreeformTool::iconAlt() const {
    return "󰽉";
};

void FreeformTool::mousePressed(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        curItem = std::dynamic_pointer_cast<Freeform>(m_itemFactory->create());
        curItem->setBoundingBoxPadding(10*context->canvas().scale());
        curItem->addPoint(context->event().pos()-context->offsetPos());
        curItem->getProperty(ItemPropertyType::StrokeWidth).setValue(m_properties[ToolPropertyType::StrokeWidth]->value());
        curItem->getProperty(ItemPropertyType::StrokeColor).setValue(m_properties[ToolPropertyType::StrokeColor]->value());
        m_isDrawing = true;
    }
}

void FreeformTool::mouseMoved(ApplicationContext *context) {
    if (m_isDrawing) {
        QPainter& painter {context->overlayPainter()};

        curItem->addPoint(context->event().pos()-context->offsetPos());
        curItem->quickDraw(painter, context->offsetPos());

        context->canvas().update();
    }
}

void FreeformTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton && m_isDrawing) {
        QPainter& overlayPainter {context->overlayPainter()};
        QPainter& canvasPainter {context->canvasPainter()};

        overlayPainter.setCompositionMode(QPainter::CompositionMode_Clear);
        overlayPainter.fillRect(context->canvas().overlay()->rect(), Qt::transparent);
        overlayPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        curItem->draw(canvasPainter, context->offsetPos());
        context->quadtree().insertItem(curItem);

        m_isDrawing = false;
        context->canvas().update();
    }
}

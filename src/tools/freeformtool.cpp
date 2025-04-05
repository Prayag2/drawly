#include "freeformtool.h"
#include "../item/factory/freeformfactory.h"
#include "../context/applicationcontext.h"
#include "../item/freeform.h"
#include "../event/event.h"
#include "../canvas/canvas.h"
#include "../item/item.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"
#include "properties/propertymanager.h"
#include "properties/toolproperty.h"

FreeformTool::FreeformTool(const PropertyManager& propertyManager) {
    m_itemFactory = std::make_unique<FreeformFactory>();

    int size {5}, borderWidth {1};
    QBitmap cursorShape {size, size};
    QPen cursorPen {};
    cursorShape.fill(Qt::transparent);
    cursorPen.setWidth(borderWidth);
    cursorPen.setColor(Qt::black);
    cursorPen.setJoinStyle(Qt::MiterJoin);

    QPainter cursorPainter {&cursorShape};
    cursorPainter.setPen(cursorPen);

    cursorPainter.drawEllipse(borderWidth/2, borderWidth/2, size-borderWidth, size-borderWidth);
    m_cursor = QCursor{cursorShape, size/2, size/2};

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
        m_lastPoint = context->event().pos() + context->offsetPos();
        curItem->addPoint(m_lastPoint);
        curItem->getProperty(ItemPropertyType::StrokeWidth).setValue(m_properties[ToolPropertyType::StrokeWidth]->value());
        curItem->getProperty(ItemPropertyType::StrokeColor).setValue(m_properties[ToolPropertyType::StrokeColor]->value());
        m_isDrawing = true;
    }
}

void FreeformTool::mouseMoved(ApplicationContext *context) {
    if (m_isDrawing) {
        QPoint curPoint {context->event().pos() + context->offsetPos()};

        // distance between the two points
        int dist {static_cast<int>(std::sqrt(std::pow(m_lastPoint.x()-curPoint.x(), 2)+std::pow(m_lastPoint.y()-curPoint.y(), 2)))};

        if (dist < Freeform::minPointDistance() * context->canvas().scale()) return;

        QPainter& painter {context->overlayPainter()};

        m_lastPoint = curPoint;
        curItem->addPoint(m_lastPoint);
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

        // Uncomment to see the bounding box
        // QPen pen{}; pen.setColor(Qt::white); canvasPainter.setPen(pen);
        // canvasPainter.drawRect(curItem->boundingBox().translated(-context->offsetPos()));

        QVector<std::shared_ptr<CacheCell>> dirtyCacheCells {context->cacheGrid().queryCells(curItem->boundingBox())};
        for (auto dirtyCacheCell : dirtyCacheCells) {
            dirtyCacheCell->setDirty(true);
        }

        m_isDrawing = false;
        context->canvas().update();
    }
}

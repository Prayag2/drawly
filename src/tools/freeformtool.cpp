#include "freeformtool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/factory/freeformfactory.h"
#include "../item/freeform.h"
#include "../item/item.h"
#include "properties/propertymanager.h"
#include "properties/toolproperty.h"

FreeformTool::FreeformTool(const PropertyManager& propertyManager) {
    m_itemFactory = std::make_unique<FreeformFactory>();

    int size{5}, borderWidth{1};
    QBitmap cursorShape{size, size};
    QPen cursorPen{};
    cursorShape.fill(Qt::transparent);
    cursorPen.setWidth(borderWidth);
    cursorPen.setColor(Qt::black);
    cursorPen.setJoinStyle(Qt::MiterJoin);

    QPainter cursorPainter{&cursorShape};
    cursorPainter.setPen(cursorPen);

    cursorPainter.drawEllipse(borderWidth / 2, borderWidth / 2, size - borderWidth,
                              size - borderWidth);
    m_cursor = QCursor{cursorShape, size / 2, size / 2};

    m_properties[ToolPropertyType::StrokeWidth] =
        (propertyManager.getToolProperty(ToolPropertyType::StrokeWidth));
    m_properties[ToolPropertyType::StrokeColor] =
        (propertyManager.getToolProperty(ToolPropertyType::StrokeColor));
}

QString FreeformTool::iconAlt() const {
    return "󰽉";
};

void FreeformTool::mousePressed(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        curItem = std::dynamic_pointer_cast<Freeform>(m_itemFactory->create());

        curItem->getProperty(ItemPropertyType::StrokeWidth)
            .setValue(m_properties[ToolPropertyType::StrokeWidth]->value());
        curItem->getProperty(ItemPropertyType::StrokeColor)
            .setValue(m_properties[ToolPropertyType::StrokeColor]->value());
        curItem->setBoundingBoxPadding(10 * context->canvas().scale());

        m_lastPoint = context->event().pos();

        auto& transformer{context->coordinateTransformer()};
        curItem->addPoint(transformer.viewToWorld(m_lastPoint), context->event().pressure());

        auto& painter{context->overlayPainter()};
        painter.save();
        painter.scale(context->zoomFactor(), context->zoomFactor());

        m_isDrawing = true;
    }
}

void FreeformTool::mouseMoved(ApplicationContext* context) {
    if (m_isDrawing) {
        auto& transformer{context->coordinateTransformer()};

        QPointF curPoint{context->event().pos()};

        // distance between the two points in the "view" coordinate system
        double dist{std::sqrt(std::pow(m_lastPoint.x() - curPoint.x(), 2) +
                              std::pow(m_lastPoint.y() - curPoint.y(), 2))};

        if (dist < Freeform::minPointDistance()) return;

        QPainter& painter{context->overlayPainter()};

        curItem->addPoint(transformer.viewToWorld(curPoint), context->event().pressure());
        curItem->quickDraw(painter, context->offsetPos());

        m_lastPoint = curPoint;
        context->markForUpdate();
    }
}

void FreeformTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton && m_isDrawing) {
        auto& transformer{context->coordinateTransformer()};

        QPainter& overlayPainter{context->overlayPainter()};
        context->canvas().overlay()->fill(Qt::transparent);
        overlayPainter.restore();

        QVector<std::shared_ptr<Item>> itemsAfterSplitting{curItem->split()};
        for (auto item : itemsAfterSplitting) {
            context->quadtree().insertItem(item);
            context->cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        }

        curItem.reset();

        m_isDrawing = false;
        context->markForRender();
        context->markForUpdate();
    }
}

ToolID FreeformTool::id() const {
    return ToolID::FreeformToolID;
}

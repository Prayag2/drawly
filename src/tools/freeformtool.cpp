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

        m_lastPoint =
            context->coordinateTransformer().toWorld(context->event().pos()) + context->offsetPos();

        curItem->addPoint(m_lastPoint, context->event().pressure());

        m_isDrawing = true;
    }
}

void FreeformTool::mouseMoved(ApplicationContext* context) {
    if (m_isDrawing) {
        qDebug() << "Pressure is: " << context->event().pressure();
        auto& transformer{context->coordinateTransformer()};

        QPointF curPoint{transformer.toWorld(context->event().pos()) + context->offsetPos()};

        // distance between the two points in the "view" coordinate system
        QPointF viewCurPoint{context->event().pos()};
        QPointF viewLastPoint{transformer.toView(m_lastPoint - context->offsetPos())};
        double dist{std::sqrt(std::pow(viewLastPoint.x() - viewCurPoint.x(), 2) +
                              std::pow(viewLastPoint.y() - viewCurPoint.y(), 2))};

        if (dist < Freeform::minPointDistance()) return;

        QPainter& painter{context->overlayPainter()};

        m_lastPoint = curPoint;
        curItem->addPoint(m_lastPoint, context->event().pressure());
        curItem->quickDraw(painter, context->offsetPos());

        context->canvas().update();
    }
}

void FreeformTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton && m_isDrawing) {
        auto& transformer{context->coordinateTransformer()};

        QPainter& overlayPainter{context->overlayPainter()};

        overlayPainter.setCompositionMode(QPainter::CompositionMode_Clear);
        overlayPainter.fillRect(context->canvas().overlay()->rect(), Qt::transparent);
        overlayPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        QVector<std::shared_ptr<Item>> itemsAfterSplitting{curItem->split()};
        for (auto item : itemsAfterSplitting) {
            overlayPainter.drawRect(item->boundingBox());
            context->quadtree().insertItem(item);
            context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());
        }
        curItem.reset();

        Common::renderItems(context);

        m_isDrawing = false;
        context->canvas().update();
    }
}

const bool FreeformTool::lowFpsTolerant() const {
    return false;
}

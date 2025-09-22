#include "freeformtool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/spatialcontext.h"
#include "../context/uicontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/factory/freeformfactory.h"
#include "../item/freeform.h"
#include "../item/item.h"
#include "../command/commandhistory.h"
#include "../command/insertitemcommand.h"
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
    UIContext& uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext& spatialContext{context->spatialContext()};
        RenderingContext& renderingContext{context->renderingContext()};
        CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};

        curItem = std::dynamic_pointer_cast<Freeform>(m_itemFactory->create());

        curItem->getProperty(ItemPropertyType::StrokeWidth)
            .setValue(m_properties[ToolPropertyType::StrokeWidth]->value());
        curItem->getProperty(ItemPropertyType::StrokeColor)
            .setValue(m_properties[ToolPropertyType::StrokeColor]->value());
        curItem->setBoundingBoxPadding(10 * renderingContext.canvas().scale());

        m_lastPoint = uiContext.event().pos();

        curItem->addPoint(transformer.viewToWorld(m_lastPoint), uiContext.event().pressure());

        auto& painter{renderingContext.overlayPainter()};
        painter.save();

        qreal zoom{renderingContext.zoomFactor()};
        painter.scale(zoom, zoom);

        m_isDrawing = true;
    }
}

void FreeformTool::mouseMoved(ApplicationContext* context) {
    if (m_isDrawing) {
        SpatialContext& spatialContext{context->spatialContext()};
        RenderingContext& renderingContext{context->renderingContext()};
        UIContext& uiContext{context->uiContext()};
        CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};

        QPointF curPoint{uiContext.event().pos()};

        // distance between the two points in the "view" coordinate system
        double dist{std::sqrt(std::pow(m_lastPoint.x() - curPoint.x(), 2) +
                              std::pow(m_lastPoint.y() - curPoint.y(), 2))};

        if (dist < Freeform::minPointDistance()) return;

        QPainter& painter{renderingContext.overlayPainter()};

        curItem->addPoint(transformer.viewToWorld(curPoint), uiContext.event().pressure());
        curItem->quickDraw(painter, spatialContext.offsetPos());

        m_lastPoint = curPoint;
        renderingContext.markForUpdate();
    }
}

void FreeformTool::mouseReleased(ApplicationContext* context) {
    UIContext& uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton && m_isDrawing) {
        SpatialContext& spatialContext{context->spatialContext()};
        RenderingContext& renderingContext{context->renderingContext()};
        CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};
        CommandHistory& commandHistory{spatialContext.commandHistory()};

        QPainter& overlayPainter{renderingContext.overlayPainter()};
        renderingContext.canvas().overlay()->fill(Qt::transparent);
        overlayPainter.restore();

        QVector<std::shared_ptr<Item>> itemsAfterSplitting{curItem->split()};
        commandHistory.insert(std::make_shared<InsertItemCommand>(itemsAfterSplitting));

        curItem.reset();

        m_isDrawing = false;
        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
}

ToolID FreeformTool::id() const {
    return ToolID::FreeformTool;
}

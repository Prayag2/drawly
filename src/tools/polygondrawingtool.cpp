#include "polygondrawingtool.h"

#include "../canvas/canvas.h"
#include "../command/commandhistory.h"
#include "../command/insertitemcommand.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/renderingcontext.h"
#include "../context/spatialcontext.h"
#include "../context/uicontext.h"
#include "../event/event.h"
#include "../item/factory/itemfactory.h"
#include "../item/polygon.h"
#include "../properties/widgets/propertymanager.h"

PolygonDrawingTool::PolygonDrawingTool() {
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties = { Property::StrokeWidth, Property::StrokeColor };
}

void PolygonDrawingTool::mousePressed(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};

        curItem = std::dynamic_pointer_cast<Polygon>(m_itemFactory->create());

        curItem->setProperty(Property::StrokeWidth, uiContext.propertyManager().value(Property::StrokeWidth));
        curItem->setProperty(Property::StrokeColor, uiContext.propertyManager().value(Property::StrokeColor));

        curItem->setBoundingBoxPadding(10 * renderingContext.canvas().scale());
        curItem->setStart(transformer.viewToWorld(uiContext.event().pos()));

        qreal zoom{renderingContext.zoomFactor()};

        QPainter &painter{renderingContext.overlayPainter()};
        painter.save();
        painter.scale(zoom, zoom);

        m_isDrawing = true;
    }
};

void PolygonDrawingTool::mouseMoved(ApplicationContext *context) {
    if (m_isDrawing) {
        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};
        UIContext &uiContext{context->uiContext()};

        QPainter &overlayPainter{renderingContext.overlayPainter()};

        QPointF offsetPos{spatialContext.offsetPos()};
        curItem->erase(overlayPainter, offsetPos);
        curItem->setEnd(transformer.viewToWorld(uiContext.event().pos()));
        curItem->draw(overlayPainter, offsetPos);

        renderingContext.markForUpdate();
    }
};

void PolygonDrawingTool::mouseReleased(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton && m_isDrawing) {
        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};
        CommandHistory &commandHistory{spatialContext.commandHistory()};

        commandHistory.insert(
            std::make_shared<InsertItemCommand>(QVector<std::shared_ptr<Item>>{curItem}));

        QPainter &overlayPainter{renderingContext.overlayPainter()};
        renderingContext.canvas().overlay()->fill(Qt::transparent);
        overlayPainter.restore();

        m_isDrawing = false;

        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

void PolygonDrawingTool::cleanup() {
    ApplicationContext *context{ApplicationContext::instance()};
    context->uiContext().event().setButton(Qt::LeftButton);
    mouseReleased(context);
}

ToolID PolygonDrawingTool::id() const {
    return ToolID::PolygonDrawingTool;
}

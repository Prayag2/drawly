#include "polygondrawingtool.h"
#include "../context/applicationcontext.h"
#include "../item/factory/itemfactory.h"
#include "../item/polygon.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"

PolygonDrawingTool::PolygonDrawingTool() {
    m_cursor = QCursor(Qt::CrossCursor);
}

void PolygonDrawingTool::mousePressed(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        curItem = std::dynamic_pointer_cast<Polygon>(m_itemFactory->create());
        curItem->setScale(context->canvas().scale());
        curItem->setBoundingBoxPadding(10*context->canvas().scale());
        curItem->setStart(context->event().pos()-context->offsetPos());
        m_isDrawing = true;
    }
};

void PolygonDrawingTool::mouseMoved(ApplicationContext *context) {
    if (m_isDrawing) {
        QPen eraser {context->pen()};
        QPainter& painter {context->overlayPainter()};

        eraser.setColor(Qt::transparent);
        eraser.setWidth(context->pen().width()*2);

        painter.setPen(eraser);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);

        curItem->draw(painter, context->offsetPos());
        curItem->setEnd(context->event().pos()-context->offsetPos());

        painter.setPen(context->pen());
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        curItem->draw(painter, context->offsetPos());

        context->canvas().update();
    }
};

void PolygonDrawingTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton && m_isDrawing) {
        QPainter& overlayPainter {context->overlayPainter()};
        QPainter& canvasPainter {context->canvasPainter()};

        overlayPainter.setCompositionMode(QPainter::CompositionMode_Clear);
        overlayPainter.fillRect(context->canvas().overlay()->rect(), Qt::transparent);
        overlayPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        canvasPainter.setPen(context->pen());
        curItem->draw(canvasPainter, context->offsetPos());
        context->quadtree().insertItem(curItem);

        m_isDrawing = false;
        qDebug() << "QuadTree size: " << context->quadtree().size();
        qDebug() << "QuadTree rect: " << context->quadtree().boundingBox();
        context->canvas().update();
    }
};

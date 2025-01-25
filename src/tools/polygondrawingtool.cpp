#include "polygondrawingtool.h"
#include "../context/applicationcontext.h"
#include "../item/factory/itemfactory.h"
#include "../item/polygon.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"

PolygonDrawingTool::PolygonDrawingTool() {}

void PolygonDrawingTool::mousePressed(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        curItem = dynamic_cast<Polygon*>(m_itemFactory->create());
        curItem->setStart(context->event().pos());
        m_isDrawing = true;
    }
};

void PolygonDrawingTool::mouseMoved(ApplicationContext *context) {
    qDebug() << "isDrawing: " << m_isDrawing;
    if (m_isDrawing) {
        qDebug() << "DRAWING...";
        QPen eraser (context->pen());
        QPainter painter (context->canvas().overlay());

        eraser.setColor(Qt::transparent);
        painter.setPen(eraser);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);

        curItem->draw(painter);
        curItem->setEnd(context->event().pos());

        painter.setPen(context->pen());
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        curItem->draw(painter);
        context->canvas().update();
    }
};

void PolygonDrawingTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        QPainter painter(context->canvas().overlay());
        painter.fillRect(context->canvas().overlay()->rect(), Qt::transparent);
        painter.end();
        painter.begin(context->canvas().canvas());
        painter.setPen(context->pen());
        curItem->draw(painter);
        context->quadtree().insertItem(curItem);
        m_isDrawing = false;
        context->canvas().update();
    }
};

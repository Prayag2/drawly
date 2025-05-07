#include <QDebug>
#include <QPainter>
#include "erasertool.h"
#include "../event/event.h"
#include "../context/applicationcontext.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"
#include "../item/item.h"
#include "../canvas/canvas.h"

EraserTool::EraserTool() {
    int size {20}, borderWidth {2};
    QBitmap eraserShape {size, size};
    QPen eraserPen {};
    eraserPen.setWidth(borderWidth);
    eraserPen.setColor(Qt::black);
    eraserPen.setJoinStyle(Qt::MiterJoin);

    QPainter eraserPainter {&eraserShape};
    eraserPainter.setPen(eraserPen);

    eraserPainter.drawRect(borderWidth/2, borderWidth/2, size-borderWidth, size-borderWidth);
    m_cursor = QCursor(eraserShape, 0, 0);
}

QString EraserTool::iconAlt() const {
    return "";
}

// TODO: Place this overload somewhere else
inline QRect operator/(const QRect& rect, double amount) {
    return QRect{rect.topLeft() / amount, rect.size() / amount};
}

void EraserTool::mousePressed(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isErasing = true;
    }
};

void EraserTool::mouseMoved(ApplicationContext *context) {
    if (m_isErasing) {
        QPainter& painter {context->canvasPainter()};

        QRect eraserBoundingBox {context->event().pos() / context->zoomFactor() + context->offsetPos().toPoint(), m_cursor.pixmap().size() / context->zoomFactor()};
        QVector<std::shared_ptr<Item>> toBeErased {context->quadtree().queryItems(eraserBoundingBox)};
        QVector<QRectF> dirtyRegions {};

        context->canvas().setBg(context->canvas().bg());
        for (std::shared_ptr<Item> item : toBeErased) {
            dirtyRegions.push_back(item->boundingBox());
            context->quadtree().deleteItem(item);
        }

        // QVector<std::shared_ptr<Item>> dirtyShapes {};
        // for (const QRectF& rect : dirtyRegions) {
        //     dirtyShapes += context->quadtree().queryItems(rect, true);
        // }

        QRectF viewport {context->offsetPos(), context->canvas().dimensions() / context->zoomFactor()};
        QVector<std::shared_ptr<Item>> dirtyShapes {context->quadtree().queryItems(viewport, true)};
        for (std::shared_ptr<Item> item : dirtyShapes) {
            item->draw(painter, context->offsetPos());
        }
        context->canvas().update();
    }
}

void EraserTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isErasing = false;
        context->cacheGrid().markAllDirty();
    }
}

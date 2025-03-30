#include <QDebug>
#include <QPainter>
#include "erasertool.h"
#include "../event/event.h"
#include "../context/applicationcontext.h"
#include "../data-structures/quadtree.h"
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

void EraserTool::mousePressed(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isErasing = true;
    }
};

void EraserTool::mouseMoved(ApplicationContext *context) {
    if (m_isErasing) {
        QPainter& painter {context->canvasPainter()};

        QRect eraserBoundingBox {QRect(context->event().pos()-context->offsetPos(), m_cursor.pixmap().size())};
        QVector<std::shared_ptr<Item>> toBeErased {context->quadtree().queryItems(eraserBoundingBox)};
        QVector<QRect> dirtyRegions {};

        for (std::shared_ptr<Item> item : toBeErased) {
            painter.fillRect(item->boundingBox().translated(context->offsetPos()), context->canvas().bg());
            dirtyRegions.push_back(item->boundingBox());
            context->quadtree().deleteItem(item);
        }

        QVector<std::shared_ptr<Item>> dirtyShapes {};
        for (const QRect& rect : dirtyRegions) {
            dirtyShapes += context->quadtree().queryItems(rect, true);
        }

        for (std::shared_ptr<Item> item : dirtyShapes) {
            item->draw(painter, context->offsetPos());
        }
    }
    context->canvas().update();
}

void EraserTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isErasing = false;
    }
}

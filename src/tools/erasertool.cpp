#include "erasertool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../common/utils.h"
#include "../context/applicationcontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/item.h"
#include <QDebug>
#include <QPainter>

EraserTool::EraserTool() {
    int size{20}, borderWidth{2};
    QBitmap eraserShape{size, size};
    QPen eraserPen{};
    eraserPen.setWidth(borderWidth);
    eraserPen.setColor(Qt::black);
    eraserPen.setJoinStyle(Qt::MiterJoin);

    QPainter eraserPainter{&eraserShape};
    eraserPainter.setPen(eraserPen);

    eraserPainter.drawRect(borderWidth / 2, borderWidth / 2, size - borderWidth,
                           size - borderWidth);
    m_cursor = QCursor(eraserShape, 0, 0);
}

QString EraserTool::iconAlt() const {
    return "";
}

// TODO: Place this overload somewhere else
inline QRect operator/(const QRect& rect, double amount) {
    return QRect{rect.topLeft() / amount, rect.size() / amount};
}

void EraserTool::mousePressed(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isErasing = true;
    }
};

void EraserTool::mouseMoved(ApplicationContext* context) {
    if (m_isErasing) {
        QRect eraserBoundingBox{context->event().pos(),
                                m_cursor.pixmap().size() * context->canvas().scale()};

        eraserBoundingBox = eraserBoundingBox / context->zoomFactor();
        eraserBoundingBox.translate(context->offsetPos().toPoint());

        QVector<std::shared_ptr<Item>> toBeErased{
            context->quadtree().queryItems(eraserBoundingBox)};

        if (toBeErased.empty()) {
            return;
        }

        for (std::shared_ptr<Item> item : toBeErased) {
            context->quadtree().deleteItem(item);

            QVector<std::shared_ptr<CacheCell>> dirtyCells{context->cacheGrid().queryCells(
                Common::unscaleRect(item->boundingBox(), context->zoomFactor()))};

            for (std::shared_ptr<CacheCell> cell : dirtyCells) {
                cell->setDirty(true);
            }
        }

        Common::renderItems(context);

        context->canvas().update();
    }
}

void EraserTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isErasing = false;
    }
}

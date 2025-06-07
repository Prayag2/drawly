#include "erasertool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../common/utils.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/item.h"
#include <QDebug>
#include <QPainter>

EraserTool::EraserTool() {
    // int size{20}, borderWidth{2};
    // QBitmap eraserShape{size, size};
    // QPen eraserPen{};
    // eraserPen.setWidth(borderWidth);
    // eraserPen.setColor(Qt::black);
    // eraserPen.setJoinStyle(Qt::MiterJoin);

    // QPainter eraserPainter{&eraserShape};
    // eraserPainter.setPen(eraserPen);

    // eraserPainter.drawRect(borderWidth / 2, borderWidth / 2, size - borderWidth,
    //                        size - borderWidth);
    // m_cursor = QCursor(eraserShape, 0, 0);
    m_cursor = QCursor(Qt::CrossCursor);
}

QString EraserTool::iconAlt() const {
    return "";
}

void EraserTool::mousePressed(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isErasing = true;
    }
};

void EraserTool::mouseMoved(ApplicationContext* context) {
    QPainter& overlayPainter{context->overlayPainter()};
    QPen pen; pen.setWidth(2); pen.setColor(Qt::red); overlayPainter.setPen(pen);

    overlayPainter.setCompositionMode(QPainter::CompositionMode_Source);
    overlayPainter.fillRect(m_lastRect.adjusted(-10, -10, 10, 10), Qt::transparent);
    overlayPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    // TODO: Adjustable eraser size
    m_lastRect = QRect{context->event().pos() - QPoint{8, 8}, QSize{20, 20}};
    overlayPainter.drawRect(m_lastRect);
    context->canvas().update();

    if (m_isErasing) {
        overlayPainter.fillRect(m_lastRect, QColor(255, 0, 0, 150));
        context->canvas().update();

        auto& transformer = context->coordinateTransformer();
        QRect worldEraserBoundingBox{transformer.toWorld(m_lastRect).toRect()};
        worldEraserBoundingBox.translate(context->offsetPos().toPoint());

        QVector<std::shared_ptr<Item>> toBeErased = context->quadtree().queryItems(worldEraserBoundingBox);

        // nothing to be erased
        if (toBeErased.empty()) {
            return;
        }

        for (std::shared_ptr<Item> item : toBeErased) {
            if (m_toBeErased.count(item) > 0) continue;
            item->getProperty(ItemPropertyType::StrokeColor).setValue(QColor(255, 255, 255, 50).rgba());
            m_toBeErased.insert(item);

            // context->quadtree().deleteItem(item);
            context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());
        }

        Common::renderItems(context);
    }

    overlayPainter.drawRect(m_lastRect);
    context->canvas().update();
}

void EraserTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        auto& transformer = context->coordinateTransformer();

        for (std::shared_ptr<Item> item : m_toBeErased) {
            context->quadtree().deleteItem(item);
            context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());
        }

        Common::renderItems(context);
        context->canvas().update();

        m_isErasing = false;
    }
}

const bool EraserTool::lowFpsTolerant() const {
    return true;
}

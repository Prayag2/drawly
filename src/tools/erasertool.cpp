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

void EraserTool::mousePressed(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isErasing = true;
    }
};

void EraserTool::mouseMoved(ApplicationContext* context) {
    if (m_isErasing) {
        auto& transformer = context->coordinateTransformer();
        QRect eraserBoundingBox{context->event().pos(),
                                m_cursor.pixmap().size() * context->canvas().scale()};

        QRect worldEraserBoundingBox{transformer.toWorld(eraserBoundingBox).toRect()};
        worldEraserBoundingBox.translate(context->offsetPos().toPoint());

        QVector<std::shared_ptr<Item>> toBeErased = context->quadtree().queryItems(worldEraserBoundingBox);

        // nothing to be erased
        if (toBeErased.empty()) {
            return;
        }

        // m_toBeErased.insert(toBeErased.begin(), toBeErased.end());
        for (std::shared_ptr<Item> item : toBeErased) {
            // item->getProperty(ItemPropertyType::StrokeColor).setValue(QColor(255, 255, 255, 50).rgba());
            context->quadtree().deleteItem(item);
            context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());
        }

        Common::renderItems(context);
        context->canvas().update();
    }
}

void EraserTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        // auto& transformer = context->coordinateTransformer();

        // for (std::shared_ptr<Item> item : m_toBeErased) {
        //     context->quadtree().deleteItem(item);
        //     context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());
        // }

        // Common::renderItems(context);
        // context->canvas().update();

        m_isErasing = false;
    }
}

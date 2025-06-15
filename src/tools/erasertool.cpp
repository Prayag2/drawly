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
    auto& transformer = context->coordinateTransformer();

    QPainter& overlayPainter{context->overlayPainter()};

    // Erase previous box
    overlayPainter.save();
    overlayPainter.setCompositionMode(QPainter::CompositionMode_Source);
    overlayPainter.fillRect(transformer.toWorld(m_lastRect.adjusted(-10, -10, 10, 10)), Qt::transparent);
    overlayPainter.restore();

    // TODO: Adjustable eraser size, remove magic numbers
    QRect curRect{context->event().pos() - QPoint{8, 8}, QSize{20, 20}};
    QRectF worldEraserRect{transformer.toWorld(curRect)};

    QRectF updateRegion{curRect.united(m_lastRect)};
    double scale{context->canvas().scale()};
    updateRegion.translate(updateRegion.topLeft() * (1 - scale) / scale);

    bool requiresFullUpdate{false};
    if (m_isErasing) {
        QVector<std::shared_ptr<Item>> toBeErased =
            context->quadtree().queryItems(worldEraserRect.translated(context->offsetPos().toPoint()));

        for (std::shared_ptr<Item> item : toBeErased) {
            if (m_toBeErased.count(item) > 0) continue;
            requiresFullUpdate = true;

            // TODO: Remove magic numbers
            item->getProperty(ItemPropertyType::StrokeColor)
                .setValue(QColor(255, 255, 255, 50).rgba());

            QRectF dirtyBox{transformer.toView(item->boundingBox().translated(-context->offsetPos()))};
            dirtyBox.translate(dirtyBox.topLeft() * (1 - scale) / scale);
            updateRegion |= dirtyBox;

            m_toBeErased.insert(item);
            context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());
        }

        Common::renderItems(context);

        overlayPainter.fillRect(worldEraserRect, QColor(255, 0, 0, 150));
    }

    // Draw eraser box
    overlayPainter.save();
    QPen pen{Qt::red, 2.0 / context->zoomFactor()};
    overlayPainter.setPen(pen);
    overlayPainter.drawRect(worldEraserRect);
    overlayPainter.restore();

    context->canvas().update(updateRegion.toRect().adjusted(-20, -20, 20, 20));
    m_lastRect = curRect;
}

void EraserTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        auto& transformer = context->coordinateTransformer();

        for (std::shared_ptr<Item> item : m_toBeErased) {
            if (context->selectedItems().count(item) > 0) {
                context->selectedItems().erase(item);
            }

            context->quadtree().deleteItem(item);
            context->cacheGrid().markDirty(transformer.toView(item->boundingBox()).toRect());
        }

        Common::renderItems(context);
        context->canvas().update();

        m_toBeErased.clear();
        m_isErasing = false;
    }
}

const bool EraserTool::lowFpsTolerant() const {
    return false;
}

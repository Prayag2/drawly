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

// FIXME: messy code
void EraserTool::mouseMoved(ApplicationContext* context) {
    auto& transformer = context->coordinateTransformer();

    QPainter& overlayPainter{context->overlayPainter()};

    // Erase previous box
    overlayPainter.save();
    overlayPainter.setCompositionMode(QPainter::CompositionMode_Source);
    overlayPainter.fillRect(m_lastRect.adjusted(-10, -10, 10, 10), Qt::transparent);

    // TODO: Adjustable eraser size, remove magic numbers
    QRect curRect{context->event().pos() - QPoint{8, 8}, QSize{20, 20}};
    QRectF worldEraserRect{transformer.viewToWorld(curRect)};

    if (m_isErasing) {
        QVector<std::shared_ptr<Item>> toBeErased =
            context->quadtree().queryItems(worldEraserRect);

        for (std::shared_ptr<Item> item : toBeErased) {
            if (m_toBeErased.count(item) > 0) continue;

            // TODO: Remove magic numbers
            item->getProperty(ItemPropertyType::StrokeColor)
                .setValue(QColor(255, 255, 255, 50).rgba());

            m_toBeErased.insert(item);
            context->cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
            context->markForRender();
        }

        overlayPainter.fillRect(curRect, QColor(255, 0, 0, 150));
    }

    context->markForUpdate();

    // Draw eraser box
    QPen pen{Qt::red, 2.0};
    overlayPainter.setPen(pen);
    overlayPainter.drawRect(curRect);
    overlayPainter.restore();

    context->markForUpdate();

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
            context->cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        }

        context->markForRender();
        context->markForUpdate();

        m_toBeErased.clear();
        m_isErasing = false;
    }
}

const bool EraserTool::lowFpsTolerant() const {
    return false;
}

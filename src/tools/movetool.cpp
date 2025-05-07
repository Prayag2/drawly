#include "movetool.h"
#include "../context/applicationcontext.h"
#include "../item/item.h"
#include "../event/event.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"

MoveTool::MoveTool() {
    m_cursor = QCursor(Qt::OpenHandCursor);
}

QString MoveTool::iconAlt() const {
    return "";
}

void MoveTool::mousePressed(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isActive = true;
        m_timer.restart();
        m_interval.restart();
        counter = 0;

        m_initialOffsetPos = context->offsetPos();
        m_initialPos = context->event().pos();
    }
};

// TODO: Place this overload somewhere else
inline QRectF operator/(const QRectF& rect, double amount) {
    return QRectF{rect.topLeft() / amount, rect.size() / amount};
}

void MoveTool::mouseMoved(ApplicationContext *context) {
    if (m_isActive) {
        // if (m_interval.elapsed() < 1000/context->fps()-1) return;
        // m_interval.restart();

        // counter++;
        // if (m_timer.elapsed() >= 1000) {
        //     counter = 0;
        //     m_timer.restart();
        // }

        QPointF newPoint {m_initialOffsetPos * context->zoomFactor() - context->event().pos() + m_initialPos};
        QPoint roundedPoint {static_cast<int>(std::round(newPoint.x())), static_cast<int>(std::round(newPoint.y()))};

        context->setOffsetPos(newPoint / context->zoomFactor());
        context->canvas().setBg(context->canvas().bg());

        QPainter& painter {context->canvasPainter()};
        QRect viewport {newPoint.toPoint(), context->canvas().dimensions()};
        QVector<std::shared_ptr<CacheCell>> visibleCells {context->cacheGrid().queryCells(viewport)};

        for (auto cell : visibleCells) {
            // UNCOMMENT TO SEE THE CACHE GRID IN ACTION!!
            // QPen pen{}; pen.setColor(Qt::white); pen.setWidth(1);
            // painter.setPen(pen);
            // painter.drawRect(cell->rect().toRectF().translated(-roundedPoint) / context->zoomFactor());
            // painter.drawText(cell->rect().toRectF().translated(-roundedPoint) / context->zoomFactor(), QString::asprintf("(%d, %d)", cell->point().x(), cell->point().y()));

            if (cell->dirty()) {
                cell->image().fill(Qt::transparent);
                cell->setDirty(false);

                QVector<std::shared_ptr<Item>> intersectingItems {
                    context->quadtree().queryItems(cell->rect() / context->zoomFactor(), true)
                };
                if (intersectingItems.empty()) continue;

                cell->painter().resetTransform();
                cell->painter().scale(context->zoomFactor(), context->zoomFactor());
                for (auto intersectingItem : intersectingItems) {
                    QPointF p {cell->rect().toRectF().topLeft() / context->zoomFactor()};
                    intersectingItem->draw(cell->painter(), p);
                }
            }
            painter.drawImage(cell->rect().toRectF().translated(-roundedPoint) / context->zoomFactor(), cell->image());
        }

        QPen p; p.setColor(Qt::white);
        painter.setPen(p);
        context->quadtree().draw(painter, -context->offsetPos());
        context->canvas().update();
    }
};

void MoveTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isActive = false;
    }
};

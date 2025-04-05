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
        m_initialOffsetPos = context->offsetPos();
        m_initialPos = context->event().pos();
        m_timer.restart();
        m_interval.restart();
        counter = 0;
    }
};

void MoveTool::mouseMoved(ApplicationContext *context) {
    if (m_isActive) {
        if (m_interval.elapsed() < 1000/context->fps()-1) return;
        m_interval.restart();

        counter++;
        if (m_timer.elapsed() >= 1000) {
            qDebug() << "Count: " << counter;
            counter = 0;
            m_timer.restart();
        }

        QPoint oldPoint {context->offsetPos()};
        QPoint newPoint {m_initialOffsetPos-context->event().pos()+m_initialPos};
        context->setOffsetPos(newPoint);

        QPainter& painter {context->canvasPainter()};

        context->canvas().setBg(context->canvas().bg());

        QRect viewport {newPoint, context->canvas().dimensions()};
        QVector<std::shared_ptr<CacheCell>> visibleCells {context->cacheGrid().queryCells(viewport)};

        for (auto cell : visibleCells) {
            // UNCOMMENT TO SEE THE CACHE GRID IN ACTION!!
            // QPen pen{}; pen.setColor(Qt::white); pen.setWidth(1);
            // painter.setPen(pen);
            // painter.drawRect(cell->rect().translated(-newPoint));
            // painter.drawText(cell->rect().translated(-newPoint), QString::asprintf("(%d, %d)", cell->point().x(), cell->point().y()));

            if (cell->dirty()) {
                QVector<std::shared_ptr<Item>> intersectingItems {context->quadtree().queryItems(cell->rect(), true)};
                if (intersectingItems.empty()) continue;

                cell->image().fill(Qt::transparent);
                for (auto intersectingItem : intersectingItems) {
                    intersectingItem->draw(cell->painter(), cell->rect().topLeft());
                }
                cell->setDirty(false);
            }
            painter.drawImage(cell->rect().translated(-newPoint), cell->image());
        }

        context->canvas().update();
    }
};

void MoveTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isActive = false;
    }
};

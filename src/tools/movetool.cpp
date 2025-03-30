#include "movetool.h"
#include "../context/applicationcontext.h"
#include "../item/item.h"
#include "../event/event.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"

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
    }
};

void MoveTool::mouseMoved(ApplicationContext *context) {
    if (m_isActive) {
        if (m_timer.elapsed() < 1000/context->fps()) return; // 2ms is extra time which could be spent in processing
        m_timer.restart();

        QPoint oldPoint {context->offsetPos()};
        QPoint newPoint {m_initialOffsetPos+(context->event().pos()-m_initialPos)};
        context->setOffsetPos(newPoint);

        QRect viewport {newPoint*-1, context->canvas().dimensions()};
        QVector<std::shared_ptr<Item>> dirty {context->quadtree().queryItems(viewport, true)};

        QPainter& painter {context->canvasPainter()};

        context->canvas().setBg(context->canvas().bg());
        for (const std::shared_ptr<Item> item : dirty) {
            item->draw(painter, newPoint);
        }

        context->canvas().update();
    }
};

void MoveTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isActive = false;
    }
};

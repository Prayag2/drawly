#include "movetool.h"
#include "../context/applicationcontext.h"
#include "../item/item.h"

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
        m_itemsToBeRedrawn = context->quadtree().getAllItems();
    }
};

void MoveTool::mouseMoved(ApplicationContext *context) {
    if (m_isActive) {
        QPoint newPoint {m_initialOffsetPos+(context->event().pos()-m_initialPos)};
        context->setOffsetPos(newPoint);

        QPainter& painter {context->canvasPainter()};
        QPen pen {context->pen()};
        painter.setPen(pen);

        // redraw all shapes (brute force, just for testing)
        context->canvas().setBg(context->canvas().bg());
        for (const std::shared_ptr<Item> item : m_itemsToBeRedrawn) {
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

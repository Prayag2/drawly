#include "movetool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../event/event.h"
#include "../item/item.h"

MoveTool::MoveTool() {
    m_cursor = QCursor(Qt::OpenHandCursor);
}

QString MoveTool::iconAlt() const {
    return "";
}

void MoveTool::mousePressed(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isActive = true;

        m_initialOffsetPos = context->offsetPos();
        m_initialPos = context->event().pos();
    }
};

void MoveTool::mouseMoved(ApplicationContext* context) {
    if (m_isActive) {
        QPointF newPoint{m_initialOffsetPos * context->zoomFactor() - context->event().pos() +
                         m_initialPos};

        context->setOffsetPos(newPoint / context->zoomFactor());

        context->canvas().setCursor(Qt::ClosedHandCursor);

        // context->canvas().update();
        context->markForRender();
        context->markForUpdate();
    }
};

void MoveTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        context->canvas().setCursor(Qt::OpenHandCursor);
        m_isActive = false;
    }
};

ToolID MoveTool::id() const {
    return ToolID::MoveToolID;
}

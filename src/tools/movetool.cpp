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
        m_lastTime = QDateTime::currentMSecsSinceEpoch();
    }
};

void MoveTool::mouseMoved(ApplicationContext* context) {
    if (m_isActive) {
        qint64 curTime = QDateTime::currentMSecsSinceEpoch();
        if (curTime - m_lastTime < 1000 / context->fps()) return;

        QPointF newPoint{m_initialOffsetPos * context->zoomFactor() - context->event().pos() +
                         m_initialPos};

        context->setOffsetPos(newPoint / context->zoomFactor());
        Common::renderItems(context);

        context->canvas().update();
        m_lastTime = QDateTime::currentMSecsSinceEpoch();
    }
};

void MoveTool::mouseReleased(ApplicationContext* context) {
    if (context->event().button() == Qt::LeftButton) {
        m_isActive = false;
    }
};

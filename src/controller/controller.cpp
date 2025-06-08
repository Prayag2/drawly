#include "controller.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../components/toolbar.h"
#include "../context/applicationcontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/item.h"
#include "../tools/tool.h"
#include <QWheelEvent>

Controller::Controller(QObject* parent) : QObject{parent} {
}

void Controller::setContext(ApplicationContext* context) {
    m_context = context;
}

void Controller::mousePressed(QMouseEvent* event) {
    if (event->pos() == QPoint{0, 0}) return;

    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());
    m_context->toolBar().curTool().mousePressed(m_context);

    if (event->type() != QEvent::TabletPress) {
        m_context->event().setPressure(1.0);
    }
    m_lastTime = QDateTime::currentMSecsSinceEpoch();
}

void Controller::mouseMoved(QMouseEvent* event) {
    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());

    qint64 curTime = QDateTime::currentMSecsSinceEpoch();
    if (curTime - m_lastTime < 1000 / m_context->fps()) {
        if (m_context->toolBar().curTool().lowFpsTolerant()) return;
    }

    m_context->toolBar().curTool().mouseMoved(m_context);

    m_lastTime = QDateTime::currentMSecsSinceEpoch();
}

void Controller::mouseReleased(QMouseEvent* event) {
    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());
    m_context->toolBar().curTool().mouseReleased(m_context);
}

void Controller::tablet(QTabletEvent* event) {
    // TODO: Remove magic numbers
    m_context->event().setPressure(event->pressure() / 1.60 + 0.375);
}

void Controller::wheel(QWheelEvent* event) {
    m_context->setOffsetPos(m_context->offsetPos() - event->pixelDelta() / m_context->zoomFactor());
    Common::renderItems(m_context);
    m_context->canvas().update();
}

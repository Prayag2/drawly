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
    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());
    m_context->toolBar().curTool().mousePressed(m_context);
}

void Controller::mouseMoved(QMouseEvent* event) {
    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());
    m_context->toolBar().curTool().mouseMoved(m_context);
}

void Controller::mouseReleased(QMouseEvent* event) {
    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());
    m_context->toolBar().curTool().mouseReleased(m_context);
}

void Controller::wheel(QWheelEvent* event) {
    m_context->setOffsetPos(m_context->offsetPos() - event->pixelDelta() / m_context->zoomFactor());
    Common::renderItems(m_context);
    m_context->canvas().update();
}

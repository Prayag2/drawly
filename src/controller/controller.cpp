#include <QWheelEvent>
#include "../item/item.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "controller.h"
#include "../canvas/canvas.h"
#include "../context/applicationcontext.h"
#include "../components/toolbar.h"
#include "../tools/tool.h"
#include "../event/event.h"

Controller::Controller(QObject *parent) : QObject{parent} {}

void Controller::setContext(ApplicationContext *context) {
    m_context = context;
}

void Controller::mousePressed(QMouseEvent *event) {
    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());
    m_context->toolBar().curTool().mousePressed(m_context);
}

void Controller::mouseMoved(QMouseEvent *event) {
    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());
    m_context->toolBar().curTool().mouseMoved(m_context);
}

void Controller::mouseReleased(QMouseEvent *event) {
    m_context->event().setPos(event->pos(), m_context->canvas().scale());
    m_context->event().setButton(event->button());
    m_context->toolBar().curTool().mouseReleased(m_context);
}

void Controller::wheel(QWheelEvent *event) {
    m_context->setOffsetPos(m_context->offsetPos() - event->pixelDelta());
    m_context->canvas().update();
    m_context->canvas().setBg(m_context->canvas().bg());

    QRect scaledViewport {m_context->offsetPos().toPoint(), m_context->canvas().dimensions() / m_context->zoomFactor()};
    m_context->cacheGrid().markAllDirty();
    QVector<std::shared_ptr<Item>> items {m_context->quadtree().queryItems(scaledViewport, true)};
    for (auto item : items) {
        item->draw(m_context->canvasPainter(), m_context->offsetPos());
    }
    m_context->canvas().update();
}

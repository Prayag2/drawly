#include "controller.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../components/toolbar.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/spatialcontext.h"
#include "../context/uicontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/item.h"
#include "../tools/tool.h"
#include <QWheelEvent>

Controller::Controller(QObject *parent) : QObject{parent} {
}

Controller::~Controller() {
    qDebug() << "Object deleted: Controller";
}

void Controller::setContext(ApplicationContext *context) {
    m_context = context;
}

void Controller::mousePressed(QMouseEvent *event) {
    if (event->pos() == QPoint{0, 0})
        return;

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    toolBar.curTool().mousePressed(m_context);

    if (event->type() != QEvent::TabletPress) {
        contextEvent.setPressure(1.0);
    }
    m_lastTime = QDateTime::currentMSecsSinceEpoch();
}

void Controller::mouseMoved(QMouseEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());

    toolBar.curTool().mouseMoved(m_context);

    m_lastTime = QDateTime::currentMSecsSinceEpoch();
}

void Controller::mouseReleased(QMouseEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    toolBar.curTool().mouseReleased(m_context);
}

void Controller::tablet(QTabletEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};

    // TODO: Remove magic numbers
    contextEvent.setPressure(event->pressure() / 1.60 + 0.375);
}

void Controller::keyPressed(QKeyEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};

    contextEvent.setKey(event->key());
    contextEvent.setModifiers(event->modifiers());
    contextEvent.setText(event->text());

    toolBar.curTool().keyPressed(m_context);
}

void Controller::keyReleased(QKeyEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};

    contextEvent.setKey(event->key());
    contextEvent.setModifiers(event->modifiers());
    contextEvent.setText(event->text());

    toolBar.curTool().keyReleased(m_context);
}

void Controller::inputMethodInvoked(QInputMethodEvent *event) {
}

// FIXME: Does not work
void Controller::wheel(QWheelEvent *event) {
    const QPointF &offsetPos{m_context->spatialContext().offsetPos()};
    const qreal zoomFactor{m_context->renderingContext().zoomFactor()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    m_context->spatialContext().setOffsetPos(offsetPos - event->pixelDelta() / zoomFactor);
    Common::renderCanvas(m_context);

    canvas.update();
}

#include "controller.h"
#include "../context/applicationcontext.h"
#include "../components/toolbar.h"
#include "../tools/tool.h"
#include "../event/event.h"

Controller::Controller(QObject *parent) : QObject{parent} {}

void Controller::setContext(ApplicationContext *context) {
    m_context = context;
}

void Controller::mousePressed(QMouseEvent *event) {
    qDebug() << "Clicked: " << event->pos();
    m_context->event().setPos(event->pos());
    m_context->event().setButton(event->button());
    m_context->toolbar().getCurTool().mousePressed(m_context);
}

void Controller::mouseMoved(QMouseEvent *event) {
    qDebug() << "Moved: " << event->pos();
    m_context->event().setPos(event->pos());
    m_context->event().setButton(event->button());
    m_context->toolbar().getCurTool().mouseMoved(m_context);
}

void Controller::mouseReleased(QMouseEvent *event) {
    qDebug() << "Released: " << event->pos();
    m_context->event().setPos(event->pos());
    m_context->event().setButton(event->button());
    m_context->toolbar().getCurTool().mouseReleased(m_context);
}

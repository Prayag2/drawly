#include "tool.h"

#include <QDebug>

Tool::Tool() {
}

Tool::~Tool() {
    qDebug() << "Object deleted: Tool";
}

const QCursor &Tool::cursor() const {
    return m_cursor;
}

const QVector<Property::Type> Tool::properties() const {
    return m_properties;
}


void Tool::mousePressed(ApplicationContext *context) {}
void Tool::mouseMoved(ApplicationContext *context) {}
void Tool::mouseReleased(ApplicationContext *context) {}
void Tool::keyPressed(ApplicationContext *context) {}
void Tool::keyReleased(ApplicationContext *context) {}
void Tool::mouseDoubleClick(ApplicationContext *context) {}
void Tool::mouseTripleClick(ApplicationContext *context) {}
void Tool::leave(ApplicationContext *context) {}
void Tool::cleanup() {}

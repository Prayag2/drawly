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

const QVector<std::shared_ptr<ToolProperty>> Tool::properties() const {
    QVector<std::shared_ptr<ToolProperty>> props{};
    for (const auto &prop : m_properties) {
        props.push_back(prop.second);
    }
    return props;
}


void Tool::mousePressed(ApplicationContext *context) {}
void Tool::mouseMoved(ApplicationContext *context) {}
void Tool::mouseReleased(ApplicationContext *context) {}
void Tool::keyPressed(ApplicationContext *context) {}
void Tool::keyReleased(ApplicationContext *context) {}
void Tool::mouseDoubleClick(ApplicationContext *context) {}
void Tool::mouseTripleClick(ApplicationContext *context) {}

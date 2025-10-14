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

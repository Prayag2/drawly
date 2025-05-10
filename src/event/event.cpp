#include "event.h"

Event::Event() {
}

void Event::setPos(const QPoint& point, qreal const scale) {
    m_pos = point * scale;
}

void Event::setButton(Qt::MouseButton btn) {
    m_button = btn;
}

const QPoint& Event::pos() const {
    return m_pos;
};

Qt::MouseButton Event::button() const {
    return m_button;
}

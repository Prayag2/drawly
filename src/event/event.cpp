#include "event.h"

Event::Event() {}

void Event::setPos(const QPoint& point) {
    m_pos = point;
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

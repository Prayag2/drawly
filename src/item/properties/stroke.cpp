#include "stroke.h"

Stroke::Stroke() {}

int Stroke::width() const {
    return m_width;
}

void Stroke::setWidth(int width) {
    m_width = width;
}

const QColor& Stroke::color() const {
    return m_color;
}

void Stroke::setColor(const QColor& color) {
    m_color = color;
}

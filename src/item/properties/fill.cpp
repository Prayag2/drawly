#include "fill.h"

Fill::Fill() {}

const QColor& Fill::color() const {
    return m_color;
}

void Fill::setColor(const QColor& color) {
    m_color = color;
}

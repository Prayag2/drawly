#include "item.h"

// PUBLIC
Item::Item() {
    m_fill = std::make_unique<Fill>();
    m_stroke = std::make_unique<Stroke>();
    m_font = std::make_unique<Font>();
}

Stroke& Item::stroke() const {
    return *m_stroke;
}

Fill& Item::fill() const {
    return *m_fill;
}

Font& Item::font() const {
    return *m_font;
}

const QRect& Item::boundingBox() const {
    return m_boundingBox;
}

void Item::setBoundingBoxPadding(int padding) {
    m_boundingBoxPadding = padding;
}

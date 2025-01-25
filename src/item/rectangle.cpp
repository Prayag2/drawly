#include "rectangle.h"

Rectangle::Rectangle() {}

void Rectangle::draw(QPainter& painter) const {
    painter.drawRect(QRect(start(), end()));
}

bool Rectangle::intersects(const QRect& rect) {
    return m_boundingBox.intersects(rect);
};

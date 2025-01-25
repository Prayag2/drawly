#include "ellipse.h"

Ellipse::Ellipse() {}

void Ellipse::draw(QPainter& painter) const {
    painter.drawEllipse(QRect(start(), end()));
}

bool Ellipse::intersects(const QRect& rect) {
    return m_boundingBox.intersects(rect);
};

#include "line.h"

#include "../common/utils.h"

Line::Line() {
}

void Line::m_draw(QPainter &painter, const QPointF &offset) const {
    painter.drawLine(start() - offset, end() - offset);
}

bool Line::intersects(const QRectF &rect) {
    return Common::intersects(rect, QLineF{start(), end()});
};

bool Line::intersects(const QLineF &line) {
    return Common::intersects(QLineF{start(), end()}, line);
}

Item::Type Line::type() const {
    return Item::Line;
}

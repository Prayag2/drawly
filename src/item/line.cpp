#include "line.h"

#include "../common/utils.h"

LineItem::LineItem() {
}

void LineItem::m_draw(QPainter &painter, const QPointF &offset) const {
    painter.drawLine(start() - offset, end() - offset);
}

bool LineItem::intersects(const QRectF &rect) {
    return Common::intersects(rect, QLineF{start(), end()});
};

bool LineItem::intersects(const QLineF &line) {
    return Common::intersects(QLineF{start(), end()}, line);
}

Item::Type LineItem::type() const {
    return Item::Line;
}

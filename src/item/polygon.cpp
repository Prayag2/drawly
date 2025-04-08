#include "polygon.h"

Polygon::Polygon() {
    m_properties[ItemPropertyType::StrokeWidth] = ItemProperty(1);
    m_properties[ItemPropertyType::StrokeColor] = ItemProperty(static_cast<int>(Qt::black));
}

void Polygon::setStart(QPointF start) {
    m_start = start;
    m_end = start;
    m_boundingBox = QRectF{start, start};
}

void Polygon::setEnd(QPointF end) {
    m_end = end;
    m_updateBoundingBox();
}

const QPointF& Polygon::start() const {
    return m_start;
}

const QPointF& Polygon::end() const {
    return m_end;
}

void Polygon::m_updateBoundingBox() {
    double minX {std::min(m_start.x(), m_end.x())};
    double maxX {std::max(m_start.x(), m_end.x())};
    double minY {std::min(m_start.y(), m_end.y())};
    double maxY {std::max(m_start.y(), m_end.y())};
    int w {m_boundingBoxPadding + getProperty(ItemPropertyType::StrokeWidth).value().toInt()};

    m_boundingBox = QRectF{QPointF{minX, maxY}, QPointF{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

void Polygon::draw(QPainter& painter, const QPointF& offset) {
    QPen pen {};
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    pen.setColor(QColor{static_cast<QRgb>(getProperty(ItemPropertyType::StrokeColor).value().toInt())});
    painter.setPen(pen);

    m_draw(painter, offset);
}

void Polygon::erase(QPainter& painter, const QPointF& offset) const {
    QPen pen {};
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt() * 10);
    pen.setColor(Qt::transparent);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    m_draw(painter, offset);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
}

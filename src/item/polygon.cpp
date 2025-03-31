#include "polygon.h"

Polygon::Polygon() {
    m_properties[ItemPropertyType::StrokeWidth] = ItemProperty(1);
    m_properties[ItemPropertyType::StrokeColor] = ItemProperty(static_cast<int>(Qt::black));
}

void Polygon::setStart(QPoint start) {
    m_start = start;
    m_end = start;
    m_boundingBox = QRect{start, start};
}

void Polygon::setEnd(QPoint end) {
    m_end = end;
    m_updateBoundingBox();
}

const QPoint& Polygon::start() const {
    return m_start;
}

const QPoint& Polygon::end() const {
    return m_end;
}

void Polygon::m_updateBoundingBox() {
    int minX {std::min(m_start.x(), m_end.x())};
    int maxX {std::max(m_start.x(), m_end.x())};
    int minY {std::min(m_start.y(), m_end.y())};
    int maxY {std::max(m_start.y(), m_end.y())};
    int w {m_boundingBoxPadding + getProperty(ItemPropertyType::StrokeWidth).value().toInt()};

    m_boundingBox = QRect{QPoint{minX, maxY}, QPoint{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

void Polygon::draw(QPainter& painter, const QPoint& offset) {
    QPen pen {};
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    pen.setColor(QColor{static_cast<QRgb>(getProperty(ItemPropertyType::StrokeColor).value().toInt())});
    painter.setPen(pen);

    m_draw(painter, offset);
}

void Polygon::erase(QPainter& painter, const QPoint& offset) const {
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

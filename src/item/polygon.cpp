#include "polygon.h"

Polygon::Polygon() {
    m_properties[ItemProperty::StrokeWidth] = ItemProperty(1);
    m_properties[ItemProperty::StrokeColor] = ItemProperty(QColor(Qt::black).rgba());
    m_properties[ItemProperty::Opacity] = ItemProperty(255);
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

const QPointF &Polygon::start() const {
    return m_start;
}

const QPointF &Polygon::end() const {
    return m_end;
}

void Polygon::m_updateBoundingBox() {
    double minX{std::min(m_start.x(), m_end.x())};
    double maxX{std::max(m_start.x(), m_end.x())};
    double minY{std::min(m_start.y(), m_end.y())};
    double maxY{std::max(m_start.y(), m_end.y())};
    int w{getProperty(ItemProperty::StrokeWidth).value().toInt()};

    m_boundingBox = QRectF{QPointF{minX, maxY}, QPointF{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

void Polygon::draw(QPainter &painter, const QPointF &offset) {
    QPen pen{};

    QColor color{QColor::fromRgba(getProperty(ItemProperty::StrokeColor).value().toUInt())};
    color.setAlpha(getProperty(ItemProperty::Opacity).value().toInt());

    pen.setWidth(getProperty(ItemProperty::StrokeWidth).value().toInt());
    pen.setColor(color);

    painter.setPen(pen);

    m_draw(painter, offset);
}

void Polygon::erase(QPainter &painter, const QPointF &offset, QColor color) const {
    QPen pen{};

    pen.setWidth(getProperty(ItemProperty::StrokeWidth).value().toInt() * 10);
    pen.setColor(color);
    painter.setPen(pen);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    m_draw(painter, offset);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
}

void Polygon::translate(const QPointF &amount) {
    m_start += amount;
    m_end += amount;

    m_updateBoundingBox();
};

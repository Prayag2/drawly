#include "polygon.h"

Polygon::Polygon() {
    m_properties[Property::StrokeWidth] = Property{1, Property::StrokeWidth};
    m_properties[Property::StrokeColor] = Property{QColor(Qt::black), Property::StrokeColor};
    m_properties[Property::Opacity] = Property{255, Property::Opacity};
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
    int w{property(Property::StrokeWidth).value<int>()};

    m_boundingBox = QRectF{QPointF{minX, maxY}, QPointF{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

void Polygon::draw(QPainter &painter, const QPointF &offset) {
    QPen pen{};

    QColor color{property(Property::StrokeColor).value<QColor>()};
    color.setAlpha(property(Property::Opacity).value<int>());

    pen.setWidth(property(Property::StrokeWidth).value<int>());
    pen.setColor(color);

    painter.setPen(pen);

    m_draw(painter, offset);
}

void Polygon::erase(QPainter &painter, const QPointF &offset, QColor color) const {
    QPen pen{};

    pen.setWidth(property(Property::StrokeWidth).value<int>() * 10);
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

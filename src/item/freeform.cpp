#include "freeform.h"
#include "properties/itemproperty.h"
#include <memory>

Freeform::Freeform() {
    m_properties[ItemPropertyType::StrokeWidth] = ItemProperty(1);
    m_properties[ItemPropertyType::StrokeColor] = ItemProperty(QColor(Qt::black).rgba());
}

int Freeform::minPointDistance() {
    return 0;
}

void Freeform::addPoint(const QPointF& point, const qreal pressure, bool optimize) {
    QPointF newPoint{point};
    if (optimize) {
        newPoint = optimizePoint(point);
    }
    double x = newPoint.x(), y = newPoint.y();

    m_boundingBox = m_boundingBox.normalized();
    double topLeftX{m_boundingBox.topLeft().x()}, topLeftY{m_boundingBox.topLeft().y()};
    double bottomRightX{m_boundingBox.bottomRight().x()},
        bottomRightY{m_boundingBox.bottomRight().y()};
    int mg{m_boundingBoxPadding + getProperty(ItemPropertyType::StrokeWidth).value().toInt()};

    if (m_points.size() <= 1) {
        m_boundingBox.setTopLeft({x - mg, y - mg});
        m_boundingBox.setBottomRight({x + mg, y + mg});
    } else {
        m_boundingBox.setLeft(std::min(topLeftX, x - mg));
        m_boundingBox.setTop(std::min(topLeftY, y - mg));
        m_boundingBox.setRight(std::max(bottomRightX, x + mg));
        m_boundingBox.setBottom(std::max(bottomRightY, y + mg));
    }

    m_points.push_back(newPoint);
    m_pressures.push_back(pressure);
}

bool Freeform::intersects(const QRectF& rect) {
    if (!boundingBox().intersects(rect)) return false;

    qsizetype pointsSize{m_points.size()};
    if (pointsSize == 1) {
        return rect.contains(m_points[0]);
    }

    QPointF p{rect.topLeft()};
    QPointF q{rect.topRight()};
    QPointF r{rect.bottomRight()};
    QPointF s{rect.bottomLeft()};

    for (qsizetype idx{0}; idx < pointsSize - 1; idx++) {
        QLine l{m_points[idx].toPoint(), m_points[idx + 1].toPoint()};

        if (Item::linesIntersect(l, {p, q}) || Item::linesIntersect(l, {q, r}) ||
            Item::linesIntersect(l, {r, s}) || Item::linesIntersect(l, {s, q}) ||
            rect.contains(m_points[idx].toPoint()) || rect.contains(m_points[idx + 1].toPoint()))
            return true;
    }

    return false;
}

void Freeform::draw(QPainter& painter, const QPointF& offset) {
    QPen pen{};

    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    pen.setColor(
        QColor::fromRgba(getProperty(ItemPropertyType::StrokeColor).value().toUInt()));

    painter.setPen(pen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    m_draw(painter, offset);
}

void Freeform::erase(QPainter& painter, const QPointF& offset, QColor color) const {
    QPen pen{};

    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    pen.setColor(color);

    painter.setPen(pen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    m_draw(painter, offset);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
}

QPointF Freeform::optimizePoint(const QPointF& newPoint) {
    m_currentWindow.push_back(newPoint);
    m_currentWindowSum += newPoint;

    if (m_currentWindow.size() > m_bufferSize) {
        m_currentWindowSum -= m_currentWindow.front();
        m_currentWindow.pop_front();
    }

    return m_currentWindowSum / m_currentWindow.size();
}

void Freeform::quickDraw(QPainter& painter, const QPointF& offset) const {
    QPen pen{};
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(getProperty(ItemPropertyType::StrokeWidth).value().toInt() * m_pressures.back());
    pen.setColor(
        QColor{static_cast<QRgb>(getProperty(ItemPropertyType::StrokeColor).value().toInt())});
    painter.setPen(pen);

    if (m_points.size() > 1) {
        painter.drawLine(m_points[m_points.size() - 2] - offset,
                         m_points.back() - offset);
    } else {
        painter.drawPoint(m_points.back());
    }
}

void Freeform::m_draw(QPainter& painter, const QPointF& offset) const {
    qsizetype pointSize{m_points.size()};
    for (qsizetype index = 0; index < pointSize; index++) {
        QPen pen{painter.pen()};
        pen.setWidthF(getProperty(ItemPropertyType::StrokeWidth).value().toInt() * m_pressures[index]);
        painter.setPen(pen);

        if (index == 0) {
            painter.drawPoint(m_points.front() - offset);
        } else {
            painter.drawLine(m_points[index - 1] - offset, m_points[index] - offset);
        }
    }
}

int Freeform::size() const {
    return m_points.size();
}

int Freeform::maxSize() const {
    // Max number of points per freeform
    return 500;
}

// If the number of points exceeds the limit, this method can be called
// to split this freeform into multiple smaller freeforms
QVector<std::shared_ptr<Item>> Freeform::split() const {
    QVector<std::shared_ptr<Item>> items;

    qsizetype pointSize{m_points.size()};
    for (qsizetype index = 0; index < pointSize; index++) {
        if (index % maxSize() == 0) {
            // add this point to the previous freeform too
            if (!items.empty()) {
                std::shared_ptr<Freeform> last{std::static_pointer_cast<Freeform>(items.back())};
                last->addPoint(m_points[index], m_pressures[index]);
            }

            // create a copy
            std::shared_ptr<Freeform> newItem{std::make_shared<Freeform>()};
            newItem->m_properties = m_properties;
            newItem->m_boundingBoxPadding = m_boundingBoxPadding;
            newItem->m_scale = m_scale;

            items.push_back(newItem);
        }
        std::shared_ptr<Freeform> cur{std::static_pointer_cast<Freeform>(items.back())};
        cur->addPoint(m_points[index], m_pressures[index], false);
    }

    return items;
}

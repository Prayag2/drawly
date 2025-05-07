#include "freeform.h"

Freeform::Freeform() {
    m_properties[ItemPropertyType::StrokeWidth] = ItemProperty(1);
    m_properties[ItemPropertyType::StrokeColor] = ItemProperty(static_cast<int>(Qt::black));
}

int Freeform::minPointDistance() {
    return 0;
}

void Freeform::addPoint(const QPointF& point) {
    QPointF smoothenedPoint{optimizePoint(m_points, 5, point)};
    double x = smoothenedPoint.x(), y = smoothenedPoint.y();

    m_boundingBox = m_boundingBox.normalized();
    double topLeftX {m_boundingBox.topLeft().x()}, topLeftY {m_boundingBox.topLeft().y()};
    double bottomRightX {m_boundingBox.bottomRight().x()}, bottomRightY {m_boundingBox.bottomRight().y()};
    int mg {m_boundingBoxPadding + getProperty(ItemPropertyType::StrokeWidth).value().toInt()};

    if (m_points.size() <= 1) {
        m_boundingBox.setTopLeft({x-mg, y-mg});
        m_boundingBox.setBottomRight({x+mg, y+mg});
    } else {
        m_boundingBox.setLeft(std::min(topLeftX, x - mg));
        m_boundingBox.setTop(std::min(topLeftY, y - mg));
        m_boundingBox.setRight(std::max(bottomRightX, x+mg));
        m_boundingBox.setBottom(std::max(bottomRightY, y+mg));
    }

    m_optimizedPoints.push_back(smoothenedPoint);
    m_points.push_back(point);
    m_cacheDirty = true;
}

bool Freeform::intersects(const QRectF& rect) {
    if (!boundingBox().intersects(rect)) return false;

    QPointF p {rect.topLeft()};
    QPointF q {rect.topRight()};
    QPointF r {rect.bottomRight()};
    QPointF s {rect.bottomLeft()};

    qsizetype n {m_points.size()};
    for (qsizetype idx {0}; idx < n-1; idx++) {
        QLine l {m_points[idx].toPoint(), m_points[idx+1].toPoint()};

        if (
            Item::linesIntersect(l, {p, q}) ||
            Item::linesIntersect(l, {q, r}) ||
            Item::linesIntersect(l, {r, s}) ||
            Item::linesIntersect(l, {s, q}) ||
            rect.contains(m_points[idx].toPoint()) ||
            rect.contains(m_points[idx+1].toPoint())
        ) return true;
    }

    return false;
}

void Freeform::draw(QPainter& painter, const QPointF& offset) {
    // m_draw(painter, offset);
    QPointF relativeOffset {m_boundingBox.x(), m_boundingBox.y()};
    if (m_cacheDirty) {
        m_cache = std::make_unique<QImage>(m_boundingBox.width(), m_boundingBox.height(), QImage::Format_ARGB32_Premultiplied);

        QPainter cachePainter{m_cache.get()};
        cachePainter.setCompositionMode(QPainter::CompositionMode_Source);
        cachePainter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        cachePainter.fillRect(m_cache->rect(), Qt::transparent);

        m_draw(cachePainter, relativeOffset);
        m_cacheDirty = false;
    }

    painter.drawImage(relativeOffset-offset, *m_cache);
}

void Freeform::erase(QPainter& painter, const QPointF& offset) const {
    // m_draw(painter, offset);
}

QPointF Freeform::optimizePoint(QVector<QPointF>& points, int bufferSize, const QPointF& newPoint) const {
    points.push_back(newPoint);

    int pointsSize{static_cast<int>(points.size())};
    if (pointsSize < bufferSize) return newPoint;

    QPointF sum {0,0};
    for (int i = pointsSize-bufferSize; i < pointsSize; i++) {
        sum += points[i];
    }

    points.pop_back();
    return sum/bufferSize;
}

void Freeform::quickDraw(QPainter& painter, const QPointF& offset) const {
    QPen pen {};
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    pen.setColor(QColor{static_cast<QRgb>(getProperty(ItemPropertyType::StrokeColor).value().toInt())});
    painter.setPen(pen);

    if (m_points.size() > 1) {
        painter.drawLine(m_optimizedPoints[m_optimizedPoints.size()-2]-offset,m_optimizedPoints.back()-offset);
    } else {
        painter.drawPoint(m_optimizedPoints.back());
    }
}

void Freeform::m_draw(QPainter& painter, const QPointF& offset) const {
    QPen pen {};
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    pen.setColor(QColor{static_cast<QRgb>(getProperty(ItemPropertyType::StrokeColor).value().toInt())});
    painter.setPen(pen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    if (m_optimizedPoints.size() == 1) {
        painter.drawPoint(m_optimizedPoints.front() - offset);
        return;
    }

    QVector<QPointF> temp{m_optimizedPoints};
    for (QPointF& point : temp) {
        point -= offset;
    }
    painter.drawPolyline(temp);
}

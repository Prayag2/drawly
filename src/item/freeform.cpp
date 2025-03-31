#include "freeform.h"

Freeform::Freeform() {
    m_properties[ItemPropertyType::StrokeWidth] = ItemProperty(1);
    m_properties[ItemPropertyType::StrokeColor] = ItemProperty(static_cast<int>(Qt::black));
}

void Freeform::addPoint(const QPoint& point) {
    QPoint smoothenedPoint{optimizePoint(m_points, 10, point)};
    int x {point.x()}, y {point.y()};

    m_boundingBox = m_boundingBox.normalized();
    int bX {m_boundingBox.x()}, bY {m_boundingBox.y()};
    int bW {m_boundingBox.width()}, bH {m_boundingBox.height()};
    int mg {m_boundingBoxPadding /* + strokeWidth */};

    QMargins adjustments {};

    if (m_points.empty()) {
        m_boundingBox.setX(x-mg);
        m_boundingBox.setY(y-mg);
        m_boundingBox.setWidth(2*mg);
        m_boundingBox.setHeight(2*mg);
    } else {
        if (x > bX+bW-mg) adjustments.setRight(x-bX-bW+mg);
        if (x < bX+mg) adjustments.setLeft(bX-x+mg);
        if (y > bY+bH-mg) adjustments.setBottom(y-bY-bH+mg);
        if (y < bY+mg) adjustments.setTop(bY-y+mg);
    }

    m_boundingBox += adjustments;
    m_optimizedPoints.push_back(smoothenedPoint);
    m_points.push_back(point);
    m_cacheDirty = true;
}

bool Freeform::intersects(const QRect& rect) {
    if (!boundingBox().intersects(rect)) return false;

    QPoint p {rect.topLeft()};
    QPoint q {rect.topRight()};
    QPoint r {rect.bottomRight()};
    QPoint s {rect.bottomLeft()};

    qsizetype n {m_points.size()};
    for (qsizetype idx {0}; idx < n-1; idx++) {
        QLine l {m_points[idx], m_points[idx+1]};

        if (
            Item::linesIntersect(l, {p, q}) ||
            Item::linesIntersect(l, {q, r}) ||
            Item::linesIntersect(l, {r, s}) ||
            Item::linesIntersect(l, {s, q}) ||
            rect.contains(m_points[idx]) ||
            rect.contains(m_points[idx+1])
        ) return true;
    }

    return false;
}

void Freeform::draw(QPainter& painter, const QPoint& offset) {
    QPoint relativeOffset {m_boundingBox.x(), m_boundingBox.y()};
    if (m_cacheDirty) {
        m_cache = std::make_unique<QImage>(m_boundingBox.width(), m_boundingBox.height(), QImage::Format_ARGB32_Premultiplied);

        QPainter cachePainter{m_cache.get()};
        cachePainter.setCompositionMode(QPainter::CompositionMode_Source);
        cachePainter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        cachePainter.fillRect(m_cache->rect(), Qt::transparent);

        m_draw(cachePainter, relativeOffset);
        m_cache->save("/home/prayag/image.png");
        m_cacheDirty = false;
    }

    painter.drawImage(relativeOffset-offset, *m_cache);
}

void Freeform::erase(QPainter& painter, const QPoint& offset) const {
    // m_draw(painter, offset);
}

// TODO: Use a better line smoothing algorithm (may as well give an option to choose from multiple algorithms)
QPoint Freeform::optimizePoint(QVector<QPoint>& points, int bufferSize, const QPoint& newPoint) const {
    points.push_back(newPoint);

    int pointsSize{static_cast<int>(points.size())};
    if (pointsSize < bufferSize) return newPoint;

    QPoint sum {0,0};
    for (int i = pointsSize-bufferSize; i < pointsSize; i++) {
        sum += points[i];
    }

    points.pop_back();
    return sum/bufferSize;
}

void Freeform::quickDraw(QPainter& painter, const QPoint& offset) const {
    QPen pen {};
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    pen.setColor(QColor{static_cast<QRgb>(getProperty(ItemPropertyType::StrokeColor).value().toInt())});
    painter.setPen(pen);

    if (m_optimizedPoints.size() > 1) {
        painter.drawLine(m_optimizedPoints[m_optimizedPoints.size()-2]-offset,m_optimizedPoints.back()-offset);
    } else {
        painter.drawPoint(m_optimizedPoints.back());
    }
}

void Freeform::m_draw(QPainter& painter, const QPoint& offset) const {
    QPen pen {};
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    pen.setColor(QColor{static_cast<QRgb>(getProperty(ItemPropertyType::StrokeColor).value().toInt())});
    painter.setPen(pen);

    QVector<QPoint> temp{m_optimizedPoints};
    for (QPoint& point : temp) {
        point -= offset;
    }
    painter.drawPolyline(temp);
}

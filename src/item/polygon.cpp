#include "polygon.h"

Polygon::Polygon() {}

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
    int w {m_boundingBoxPadding + stroke().width()};

    m_boundingBox = QRect{QPoint{minX, maxY}, QPoint{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

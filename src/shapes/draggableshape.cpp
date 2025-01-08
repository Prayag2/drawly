#include "draggableshape.h"
#include <QDebug>

DraggableShape::DraggableShape(int strokeWidth) : Shape(strokeWidth) {}

void DraggableShape::setStart(QPoint start) {
    m_start = start;
    m_end = start;
    m_boundingBox = QRect{start, start};
}

void DraggableShape::setEnd(QPoint end) {
    m_end = end;
    updateBoundingBox();
}

const QPoint& DraggableShape::start() {
    return m_start;
}

const QPoint& DraggableShape::end() {
    return m_end;
}

void DraggableShape::updateBoundingBox() {
    int minX = std::min(m_start.x(), m_end.x());
    int maxX = std::max(m_start.x(), m_end.x());
    int minY = std::min(m_start.y(), m_end.y());
    int maxY = std::max(m_start.y(), m_end.y());
    int w {m_margin};

    m_boundingBox = QRect{QPoint{minX, maxY}, QPoint{maxX, minY}}.normalized();
    m_boundingBox.adjust(-w, -w, w, w);
}

const QRect& DraggableShape::boundingBox() const {
    return m_boundingBox;
}

DrawingMode DraggableShape::drawingMode() const {
    return DrawingMode::Drag;
}

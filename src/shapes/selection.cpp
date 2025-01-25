#include "selection.h"

Selection::Selection() : DraggableShape(0) {}

Selection::Selection(const Selection& selection) : DraggableShape(0) {
    m_start = selection.m_start;
    m_end = selection.m_end;
    m_boundingBox = selection.m_boundingBox;
}

Selection::Selection(Selection* const selection) : DraggableShape(0) {
    m_start = selection->m_start;
    m_end = selection->m_end;
    m_boundingBox = selection->m_boundingBox;
}

Selection& Selection::operator=(const Selection& selection) {
    if (this == &selection)
        return *this;

    m_boundingBox = selection.m_boundingBox;
    m_start = selection.m_start;
    m_end = selection.m_end;
    m_margin = selection.m_margin;
    return *this;
}

DrawingMode Selection::drawingMode() const {
    return DrawingMode::Drag;
}

const QRect& Selection::boundingBox() const {
    return m_boundingBox;
}

ShapeType Selection::shapeType() const {
    return ShapeType::Selection;
}

bool Selection::intersects(QRect boundingBox) const {
    return m_boundingBox.intersects(boundingBox);
}

void Selection::draw(QPainter& painter) const {
    painter.drawRect(m_boundingBox);
}

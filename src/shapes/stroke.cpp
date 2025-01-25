#include "stroke.h"

Stroke::Stroke(int strokeWidth) : Shape (strokeWidth) {}
Stroke::Stroke(const Stroke& stroke) : Shape (stroke.m_margin) {
    m_boundingBox = stroke.m_boundingBox;
    m_points = stroke.m_points;
}

Stroke::Stroke(Stroke* const stroke) : Shape (stroke->m_margin) {
    m_boundingBox = stroke->m_boundingBox;
    m_points = stroke->m_points;
}

Stroke& Stroke::operator=(const Stroke& stroke) {
    if (this == &stroke)
        return *this;

    m_boundingBox = stroke.m_boundingBox;
    m_points = stroke.m_points;
    m_margin = stroke.m_margin;
    return *this;
}

void Stroke::addPoint(const QPoint& point) {
    int x1 {point.x()}, y1 {point.y()};
    int x {m_boundingBox.x()}, y {m_boundingBox.y()};
    int w {m_boundingBox.width()}, h {m_boundingBox.height()};
    int sw {m_margin};
    QMargins margins {};

    if (m_points.size() == 0) {
        m_boundingBox.setX(point.x()-sw);
        m_boundingBox.setY(point.y()-sw);
        m_boundingBox.setWidth(2*sw);
        m_boundingBox.setHeight(2*sw);
    } else {
        if (x1 > x+w-sw) margins.setRight(x1-x-w+sw);
        if (x1 < x+sw) margins.setLeft(x-x1+sw);
        if (y1 > y+h-sw) margins.setBottom(y1-y-h+sw);
        if (y1 < y+sw) margins.setTop(y-y1+sw);
    }
    m_boundingBox += margins;

    m_points.push_back(point);
}

void Stroke::draw(QPainter& painter) const {
    int buffer {10};
    QVector<QPoint> points {m_points.front()};
    for (int i = 0; i < m_points.size()-buffer; i++) {
        QPoint avg {};
        for (int j = i; j < i+buffer; j++) {
            avg += m_points[j];
        }
        avg /= buffer;
        points.push_back(avg);
    }
    points.push_back(m_points.back());
    painter.drawPolyline(points);
}

const QRect& Stroke::boundingBox() const {
    return m_boundingBox;
}

DrawingMode Stroke::drawingMode() const {
    return DrawingMode::Sketch;
};

ShapeType Stroke::shapeType() const {
    return ShapeType::Stroke;
}

int Stroke::size() const {
    return m_points.size();
}


bool Stroke::intersects(QRect boundingBox) const {
    if (!m_boundingBox.intersects(boundingBox)) return false;

    // o(n) average case
    for (const QPoint& point : m_points) {
        if (boundingBox.contains(point)) return true;
    }
    return false;
}

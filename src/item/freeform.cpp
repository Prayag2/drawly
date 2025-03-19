#include "freeform.h"

Freeform::Freeform() {}

void Freeform::draw(QPainter& painter, const QPoint& offset) const {
    m_draw(painter, offset);
}

void Freeform::erase(QPainter& painter, const QPoint& offset) const {
    // m_draw(painter, offset);
}

void Freeform::quickDraw(QPainter& painter, const QPoint& offset) const {
    QVector<QPoint> last10Points {};

    int n {static_cast<int>(m_points.size())};
    for (int i {std::max(0, n-10)}; i < n; i++) {
        last10Points.push_back(m_points[i]+offset);
    }

    painter.drawPolyline(last10Points);
}

void Freeform::m_draw(QPainter& painter, const QPoint& offset) const {
    int buffer {12};
    QVector<QPoint> points {m_points.front()+offset};
    for (int i = 0; i < m_points.size()-buffer; i++) {
        QPoint avg {};
        for (int j = i; j < i+buffer; j++) {
            avg += m_points[j];
        }
        avg /= buffer;
        points.push_back(avg+offset);
    }
    points.push_back(m_points.back()+offset);
    painter.drawPolyline(points);
}

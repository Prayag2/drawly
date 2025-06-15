#include "arrow.h"

#include "../common/utils.h"

Arrow::Arrow() {
}

void Arrow::setStart(QPointF start) {
    Polygon::setStart(start);
    calcArrowPoints();
}

void Arrow::setEnd(QPointF end) {
    Polygon::setEnd(end);
    calcArrowPoints();
}

void Arrow::calcArrowPoints() {
    double x1{start().x()}, x2{end().x()};
    double y1{start().y()}, y2{end().y()};

    qreal angle{std::atan2(y2 - y1, x2 - x1)};
    qreal arrowLength{std::sqrt(std::pow(y2 - y1, 2) + std::pow(x2 - x1, 2))};

    int maxArrowSize{static_cast<int>(m_maxArrowSize * m_scale)};
    int arrowSize{std::min(maxArrowSize, static_cast<int>(arrowLength * 0.5))};

    m_arrowP1 = QPoint(x2 - arrowSize * std::cos(angle - (M_PI / 180) * 30),
                       y2 - arrowSize * std::sin(angle - (M_PI / 180) * 30));
    m_arrowP2 = QPoint(x2 - arrowSize * std::cos(angle + (M_PI / 180) * 30),
                       y2 - arrowSize * std::sin(angle + (M_PI / 180) * 30));
}

void Arrow::m_draw(QPainter& painter, const QPointF& offset) const {
    painter.drawLine(start() - offset, end() - offset);
    painter.drawLine(end() - offset, m_arrowP1 - offset);
    painter.drawLine(end() - offset, m_arrowP2 - offset);
}

bool Arrow::intersects(const QRectF& rect) {
    if (!boundingBox().intersects(rect)) return false;

    // TODO: Use better techniques to detect collision
    QPointF p{start()}, q{end()}, r{m_arrowP1}, s{m_arrowP2};
    QPointF a{rect.x(), rect.y()};
    QPointF b{rect.x() + rect.width(), rect.y()};
    QPointF c{rect.x() + rect.width(), rect.y() + rect.height()};
    QPointF d{rect.x(), rect.y() + rect.height()};

    return (Common::intersects(QLineF{p, q}, QLineF{a, b}) ||
            Common::intersects(QLineF{p, q}, QLineF{b, c}) ||
            Common::intersects(QLineF{p, q}, QLineF{c, d}) ||
            Common::intersects(QLineF{p, q}, QLineF{d, a}) ||
            Common::intersects(QLineF{q, r}, QLineF{a, b}) ||
            Common::intersects(QLineF{q, r}, QLineF{b, c}) ||
            Common::intersects(QLineF{q, r}, QLineF{c, d}) ||
            Common::intersects(QLineF{q, r}, QLineF{d, a}) ||
            Common::intersects(QLineF{q, s}, QLineF{a, b}) ||
            Common::intersects(QLineF{q, s}, QLineF{b, c}) ||
            Common::intersects(QLineF{q, s}, QLineF{c, d}) ||
            Common::intersects(QLineF{q, s}, QLineF{d, a}));
};

bool Arrow::intersects(const QLineF& line) {
    return (Common::intersects(QLineF{start(), end()}, line) ||
            Common::intersects(QLineF{end(), m_arrowP1}, line) ||
            Common::intersects(QLineF{end(), m_arrowP2}, line));
}

void Arrow::translate(const QPointF& amount) {
    m_arrowP1 += amount;
    m_arrowP2 += amount;

    Polygon::translate(amount);
};

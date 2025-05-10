#ifndef UTILS_H
#define UTILS_H

#include <QRectF>

inline QRectF operator/(const QRectF& rect, double amount) {
    return QRectF{rect.topLeft() / amount, rect.size() / amount};
}

namespace Common {
inline QPoint unscalePoint(const QPointF& point, qreal zoomFactor) {
    return point.toPoint() * zoomFactor;
}

inline QPointF unscalePointF(const QPointF& point, qreal zoomFactor) {
    return point * zoomFactor;
}

inline QPoint unscalePoint(const QPoint& point, qreal zoomFactor) {
    return unscalePoint(point.toPointF(), zoomFactor);
}

inline QRect unscaleRect(const QRectF& rect, qreal zoomFactor) {
    QSize boxSize{rect.size().toSize() * zoomFactor};

    return QRect{unscalePoint(rect.topLeft(), zoomFactor), boxSize};
}

inline QRect unscaleRect(const QRect& rect, qreal zoomFactor) {
    return unscaleRect(rect.toRectF(), zoomFactor);
}

inline QRectF unscaleRectF(const QRectF& rect, qreal zoomFactor) {
    QSize boxSize{rect.size().toSize() * zoomFactor};

    return QRectF{unscalePointF(rect.topLeft(), zoomFactor), boxSize};
}

inline QPoint scalePoint(const QPointF& point, qreal zoomFactor) {
    return point.toPoint() / zoomFactor;
}

inline QPointF scalePointF(const QPointF& point, qreal zoomFactor) {
    return point / zoomFactor;
}

inline QPoint scalePoint(const QPoint& point, qreal zoomFactor) {
    return scalePoint(point.toPointF(), zoomFactor);
}

inline QRect scaleRect(const QRectF& rect, qreal zoomFactor) {
    QSize boxSize{rect.size().toSize() / zoomFactor};

    return QRect{scalePoint(rect.topLeft(), zoomFactor), boxSize};
}

inline QRect scaleRect(const QRect& rect, qreal zoomFactor) {
    return scaleRect(rect.toRectF(), zoomFactor);
}

inline QRectF scaleRectF(const QRectF& rect, qreal zoomFactor) {
    QSize boxSize{rect.size().toSize() * zoomFactor};

    return QRectF{scalePointF(rect.topLeft(), zoomFactor), boxSize};
}
};  // namespace Common

#endif

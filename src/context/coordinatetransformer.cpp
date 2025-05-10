#include "coordinatetransformer.h"

#include "applicationcontext.h"
#include <cmath>

CoordinateTransformer::CoordinateTransformer(ApplicationContext* context) : m_context{context} {};

QPointF CoordinateTransformer::toView(QPointF point) {
    return point * m_context->zoomFactor();
}

QSizeF CoordinateTransformer::toView(QSizeF size) {
    return size * m_context->zoomFactor();
}

QRectF CoordinateTransformer::toView(QRectF rect) {
    return {toView(rect.topLeft()), toView(rect.size())};
}

QPointF CoordinateTransformer::toWorld(QPointF point) {
    return point / m_context->zoomFactor();
}

QSizeF CoordinateTransformer::toWorld(QSizeF size) {
    return size / m_context->zoomFactor();
}

QRectF CoordinateTransformer::toWorld(QRectF rect) {
    return {toWorld(rect.topLeft()), toWorld(rect.size())};
}

QPointF CoordinateTransformer::toView(QPoint point) {
    return toView(point.toPointF());
}

QSizeF CoordinateTransformer::toView(QSize size) {
    return toView(size.toSizeF());
}

QRectF CoordinateTransformer::toView(QRect rect) {
    return toView(rect.toRectF());
}

QPointF CoordinateTransformer::toWorld(QPoint point) {
    return toWorld(point.toPointF());
}

QSizeF CoordinateTransformer::toWorld(QSize size) {
    return toWorld(size.toSizeF());
}

QRectF CoordinateTransformer::toWorld(QRect rect) {
    return toWorld(rect.toRectF());
}

#include "coordinatetransformer.h"

#include "applicationcontext.h"
#include "renderingcontext.h"
#include "spatialcontext.h"
#include <cmath>

CoordinateTransformer::CoordinateTransformer(ApplicationContext *context)
    : m_applicationContext{context} {};

CoordinateTransformer::~CoordinateTransformer() {
    qDebug() << "Object deleted: CoordinateTransformer";
}

void CoordinateTransformer::setCoordinateTransformer() {
    m_spatialContext = &(m_applicationContext->spatialContext());
    m_renderingContext = &(m_applicationContext->renderingContext());
}

QPointF CoordinateTransformer::worldToView(QPointF point) {
    return (point - m_spatialContext->offsetPos()) * m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::worldToView(QSizeF size) {
    return size * m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::worldToView(QRectF rect) {
    return {worldToView(rect.topLeft()), worldToView(rect.size())};
}

QPointF CoordinateTransformer::viewToWorld(QPointF point) {
    return point / m_renderingContext->zoomFactor() + m_spatialContext->offsetPos();
}

QSizeF CoordinateTransformer::viewToWorld(QSizeF size) {
    return size / m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::viewToWorld(QRectF rect) {
    return {viewToWorld(rect.topLeft()), viewToWorld(rect.size())};
}

QPoint CoordinateTransformer::worldToView(QPoint point) {
    return worldToView(point.toPointF()).toPoint();
}

QSize CoordinateTransformer::worldToView(QSize size) {
    return worldToView(size.toSizeF()).toSize();
}

QRect CoordinateTransformer::worldToView(QRect rect) {
    return worldToView(rect.toRectF()).toRect();
}

QPoint CoordinateTransformer::viewToWorld(QPoint point) {
    return viewToWorld(point.toPointF()).toPoint();
}

QSize CoordinateTransformer::viewToWorld(QSize size) {
    return viewToWorld(size.toSizeF()).toSize();
}

QRect CoordinateTransformer::viewToWorld(QRect rect) {
    return viewToWorld(rect.toRectF()).toRect();
}

QPointF CoordinateTransformer::worldToGrid(QPointF point) {
    return point * m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::worldToGrid(QSizeF size) {
    return size * m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::worldToGrid(QRectF rect) {
    return {worldToGrid(rect.topLeft()), worldToGrid(rect.size())};
}

QPointF CoordinateTransformer::gridToWorld(QPointF point) {
    return point / m_renderingContext->zoomFactor();
}

QSizeF CoordinateTransformer::gridToWorld(QSizeF size) {
    return size / m_renderingContext->zoomFactor();
}

QRectF CoordinateTransformer::gridToWorld(QRectF rect) {
    return {gridToWorld(rect.topLeft()), gridToWorld(rect.size())};
}

QPoint CoordinateTransformer::worldToGrid(QPoint point) {
    return worldToGrid(point.toPointF()).toPoint();
}

QSize CoordinateTransformer::worldToGrid(QSize size) {
    return worldToGrid(size.toSizeF()).toSize();
}

QRect CoordinateTransformer::worldToGrid(QRect rect) {
    return worldToGrid(rect.toRectF()).toRect();
}

QPoint CoordinateTransformer::gridToWorld(QPoint point) {
    return gridToWorld(point.toPointF()).toPoint();
}

QSize CoordinateTransformer::gridToWorld(QSize size) {
    return gridToWorld(size.toSizeF()).toSize();
}

QRect CoordinateTransformer::gridToWorld(QRect rect) {
    return gridToWorld(rect.toRectF()).toRect();
}

QPointF CoordinateTransformer::viewToGrid(QPointF point) {
    return point + m_spatialContext->offsetPos();
}

QSizeF CoordinateTransformer::viewToGrid(QSizeF size) {
    return size;
}

QRectF CoordinateTransformer::viewToGrid(QRectF rect) {
    return QRectF{viewToGrid(rect.topLeft()), viewToGrid(rect.bottomRight())};
}

QPointF CoordinateTransformer::gridToView(QPointF point) {
    return point - m_spatialContext->offsetPos();
}

QSizeF CoordinateTransformer::gridToView(QSizeF size) {
    return size;
}

QRectF CoordinateTransformer::gridToView(QRectF rect) {
    return QRectF{gridToView(rect.topLeft()), gridToView(rect.bottomRight())};
}

QPoint CoordinateTransformer::viewToGrid(QPoint point) {
    return viewToGrid(point.toPointF()).toPoint();
}

QSize CoordinateTransformer::viewToGrid(QSize size) {
    return viewToGrid(size.toSizeF()).toSize();
}

QRect CoordinateTransformer::viewToGrid(QRect rect) {
    return viewToGrid(rect.toRectF()).toRect();
}

QPoint CoordinateTransformer::gridToView(QPoint point) {
    return gridToView(point.toPointF()).toPoint();
}

QSize CoordinateTransformer::gridToView(QSize size) {
    return gridToView(size.toSizeF()).toSize();
}

QRect CoordinateTransformer::gridToView(QRect rect) {
    return gridToView(rect.toRectF()).toRect();
}

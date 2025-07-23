#ifndef COORDINATETRANSFORMER_H
#define COORDINATETRANSFORMER_H

#include <QRect>
class ApplicationContext;

/*
 * There are three coordinate systems in drawly:
 *  1. World (the one used by the QuadTree to store items)
 *  2. Grid (the one used by the CacheGrid to cache tiles)
 *  3. View (the viewport)
 */
class CoordinateTransformer {
private:
    ApplicationContext* m_context;

public:
    CoordinateTransformer(ApplicationContext* context);

    QPointF worldToView(QPointF point);
    QSizeF worldToView(QSizeF size);
    QRectF worldToView(QRectF rect);
    QPointF viewToWorld(QPointF point);
    QSizeF viewToWorld(QSizeF size);
    QRectF viewToWorld(QRectF rect);

    QPoint worldToView(QPoint point);
    QSize worldToView(QSize size);
    QRect worldToView(QRect rect);
    QPoint viewToWorld(QPoint point);
    QSize viewToWorld(QSize size);
    QRect viewToWorld(QRect rect);

    QPointF worldToGrid(QPointF point);
    QSizeF worldToGrid(QSizeF size);
    QRectF worldToGrid(QRectF rect);
    QPointF gridToWorld(QPointF point);
    QSizeF gridToWorld(QSizeF size);
    QRectF gridToWorld(QRectF rect);

    QPoint worldToGrid(QPoint point);
    QSize worldToGrid(QSize size);
    QRect worldToGrid(QRect rect);
    QPoint gridToWorld(QPoint point);
    QSize gridToWorld(QSize size);
    QRect gridToWorld(QRect rect);

    QPointF viewToGrid(QPointF point);
    QSizeF viewToGrid(QSizeF size);
    QRectF viewToGrid(QRectF rect);
    QPointF gridToView(QPointF point);
    QSizeF gridToView(QSizeF size);
    QRectF gridToView(QRectF rect);

    QPoint viewToGrid(QPoint point);
    QSize viewToGrid(QSize size);
    QRect viewToGrid(QRect rect);
    QPoint gridToView(QPoint point);
    QSize gridToView(QSize size);
    QRect gridToView(QRect rect);
};

#endif  // COORDINATETRANSFORMER_H

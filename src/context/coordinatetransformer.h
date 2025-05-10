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

    QPointF toView(QPointF point);
    QSizeF toView(QSizeF size);
    QRectF toView(QRectF rect);
    QPointF toWorld(QPointF point);
    QSizeF toWorld(QSizeF size);
    QRectF toWorld(QRectF rect);

    QPointF toView(QPoint point);
    QSizeF toView(QSize size);
    QRectF toView(QRect rect);
    QPointF toWorld(QPoint point);
    QSizeF toWorld(QSize size);
    QRectF toWorld(QRect rect);
};

#endif  // COORDINATETRANSFORMER_H

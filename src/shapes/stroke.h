#ifndef STROKE_H
#define STROKE_H

#include "shape.h"

class Stroke : public Shape
{
public:
    Stroke(int strokeWidth);
    Stroke(const Stroke& stroke);
    Stroke(Stroke* const stroke);
    Stroke& operator=(const Stroke& stroke);

    DrawingMode drawingMode() const override;
    const QRect& boundingBox() const override;
    bool intersects(QRect boundingBox) const override;

    void addPoint(const QPoint& point);
    void draw(QPainter& painter) const override;

    ShapeType shapeType() const override;
    int size() const;
private:
    QVector<QPoint> m_points {};
};

#endif // STROKE_H

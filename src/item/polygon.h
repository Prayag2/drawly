#ifndef POLYGON_H
#define POLYGON_H

#include "item.h"

class Polygon : public Item {
public:
    Polygon();

    virtual void setStart(QPointF start);
    virtual void setEnd(QPointF end);

    void draw(QPainter& painter, const QPointF& offset) override;
    void erase(QPainter& painter, const QPointF& offset,
               QColor color = Qt::transparent) const override;

    const QPointF& start() const;
    const QPointF& end() const;

private:
    QPointF m_start{};
    QPointF m_end{};

    void m_updateBoundingBox();
};

#endif  // POLYGON_H

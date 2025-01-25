#ifndef POLYGON_H
#define POLYGON_H

#include "item.h"

class Polygon : public Item
{
public:
    Polygon();

    virtual void setStart(QPoint start);
    virtual void setEnd(QPoint end);

    const QPoint& start() const;
    const QPoint& end() const;

private:
    QPoint m_start {};
    QPoint m_end {};
    void m_updateBoundingBox();
};

#endif // POLYGON_H

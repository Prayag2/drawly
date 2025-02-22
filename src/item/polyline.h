#ifndef POLYLINE_H
#define POLYLINE_H

#include "item.h"
#include <QVector>

class Polyline : public Item {
  public:
    Polyline();
    ~Polyline() = default;

    void addPoint(const QPoint& point);
    bool intersects(const QRect& rect) override;

  protected:
    QVector<QPoint> m_points {};
};

#endif // POLYLINE_H

#ifndef FREEFORM_H
#define FREEFORM_H

#include "polyline.h"

class Freeform : public Polyline {
  public:
    Freeform();
    ~Freeform() = default;

    void draw(QPainter& painter, const QPoint& offset) const override;
    void erase(QPainter& painter, const QPoint& offset) const override;
    void quickDraw(QPainter& painter, const QPoint& offset) const;

  protected:
    void m_draw(QPainter& painter, const QPoint& offset) const override;
};

#endif // FREEFORM_H

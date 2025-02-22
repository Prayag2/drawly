#ifndef FREEFORM_H
#define FREEFORM_H

#include "polyline.h"

class Freeform : public Polyline
{
public:
    Freeform();
    ~Freeform() = default;

    void draw(QPainter& painter) const override;
    void quickDraw(QPainter& painter) const;
};

#endif // FREEFORM_H

#ifndef LINE_H
#define LINE_H

#include "draggableshape.h"

class Line : public DraggableShape
{
public:
    Line(int strokeWidth);
    Line(const Line& line);
    Line(Line* const line);
    Line& operator=(const Line& line);

    void draw(QPainter& painter) const override;
    bool intersects(QRect boundingBox) const override;
    ShapeType shapeType() const override;
private:
    int orientation(QPoint a, QPoint b, QPoint c) const;
};

#endif // LINE_H

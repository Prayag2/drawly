#ifndef POLYGONDRAWINGTOOL_H
#define POLYGONDRAWINGTOOL_H

#include "drawingtool.h"
class Polygon;

class PolygonDrawingTool : public DrawingTool
{
protected:
    Polygon* curItem;
public:
    PolygonDrawingTool();
    ~PolygonDrawingTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
};

#endif // POLYGONDRAWINGTOOL_H

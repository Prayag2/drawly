#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "polygondrawingtool.h"

class RectangleTool : public PolygonDrawingTool
{
public:
    RectangleTool();
    ~RectangleTool() override = default;

    QString iconAlt() const override;
};

#endif // RECTANGLETOOL_H

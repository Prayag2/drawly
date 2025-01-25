#ifndef ELLIPSETOOL_H
#define ELLIPSETOOL_H

#include "polygondrawingtool.h"

class EllipseTool : public PolygonDrawingTool
{
public:
    EllipseTool();

    ~EllipseTool() override = default;

    QString iconAlt() const override;
};

#endif // ELLIPSETOOL_H

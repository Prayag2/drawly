#ifndef ELLIPSETOOL_H
#define ELLIPSETOOL_H

#include "polygondrawingtool.h"
class PropertyManager;

class EllipseTool : public PolygonDrawingTool {
public:
    EllipseTool(const PropertyManager& propertyManager);

    ~EllipseTool() override = default;

    QString iconAlt() const override;
};

#endif  // ELLIPSETOOL_H

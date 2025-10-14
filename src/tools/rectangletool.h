#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "polygondrawingtool.h"
class PropertyManager;

class RectangleTool : public PolygonDrawingTool {
public:
    RectangleTool(const PropertyManager &propertyManager);
    ~RectangleTool() override = default;

    QString iconAlt() const override;
};

#endif  // RECTANGLETOOL_H

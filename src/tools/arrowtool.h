#ifndef ARROWTOOL_H
#define ARROWTOOL_H

#include "polygondrawingtool.h"
class PropertyManager;

class ArrowTool : public PolygonDrawingTool {
public:
    ArrowTool(const PropertyManager &propertyManager);
    ~ArrowTool() override = default;
    QString iconAlt() const override;
};

#endif  // ARROWTOOL_H

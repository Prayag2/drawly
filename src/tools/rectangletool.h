#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "polygondrawingtool.h"
class PropertyManager;

class RectangleTool : public PolygonDrawingTool {
public:
    RectangleTool();
    ~RectangleTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;
};

#endif  // RECTANGLETOOL_H

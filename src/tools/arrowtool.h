#ifndef ARROWTOOL_H
#define ARROWTOOL_H

#include "polygondrawingtool.h"
class PropertyManager;

class ArrowTool : public PolygonDrawingTool {
public:
    ArrowTool();
    ~ArrowTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;
};

#endif  // ARROWTOOL_H

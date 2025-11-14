#ifndef ELLIPSETOOL_H
#define ELLIPSETOOL_H

#include "polygondrawingtool.h"
class PropertyManager;

class EllipseTool : public PolygonDrawingTool {
public:
    EllipseTool();

    ~EllipseTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;
};

#endif  // ELLIPSETOOL_H

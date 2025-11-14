#ifndef LINETOOL_H
#define LINETOOL_H

#include "polygondrawingtool.h"
class PropertyManager;

class LineTool : public PolygonDrawingTool {
public:
    LineTool();
    ~LineTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;
};

#endif  // LINETOOL_H

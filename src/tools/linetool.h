#ifndef LINETOOL_H
#define LINETOOL_H

#include "polygondrawingtool.h"
class PropertyManager;

class LineTool : public PolygonDrawingTool {
public:
    LineTool();
    ~LineTool() override = default;
    QString iconAlt() const override;
};

#endif  // LINETOOL_H

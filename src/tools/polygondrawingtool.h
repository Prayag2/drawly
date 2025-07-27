#ifndef POLYGONDRAWINGTOOL_H
#define POLYGONDRAWINGTOOL_H

#include "drawingtool.h"
class Polygon;
class PropertyManager;

class PolygonDrawingTool : public DrawingTool {
protected:
    std::shared_ptr<Polygon> curItem{};

public:
    PolygonDrawingTool(const PropertyManager& propertyManager);
    ~PolygonDrawingTool() override = default;

    void mousePressed(ApplicationContext* context) override;
    void mouseMoved(ApplicationContext* context) override;
    void mouseReleased(ApplicationContext* context) override;
    ToolID id() const override;
};

#endif  // POLYGONDRAWINGTOOL_H

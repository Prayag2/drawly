#ifndef POLYGONDRAWINGTOOL_H
#define POLYGONDRAWINGTOOL_H

#include "drawingtool.h"
class PolygonItem;
class PropertyManager;

class PolygonDrawingTool : public DrawingTool {
protected:
    std::shared_ptr<PolygonItem> curItem{};

public:
    PolygonDrawingTool();
    ~PolygonDrawingTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void cleanup() override;

    Tool::Type type() const override;
};

#endif  // POLYGONDRAWINGTOOL_H

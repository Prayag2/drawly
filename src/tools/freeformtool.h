#ifndef FREEFORMTOOL_H
#define FREEFORMTOOL_H

#include "drawingtool.h"
#include <QElapsedTimer>
class Freeform;
class PropertyManager;

class FreeformTool : public DrawingTool {
public:
    FreeformTool(const PropertyManager& propertyManager);
    ~FreeformTool() override = default;

    QString iconAlt() const override;
    void mousePressed(ApplicationContext* context) override;
    void mouseMoved(ApplicationContext* context) override;
    void mouseReleased(ApplicationContext* context) override;
    ToolID id() const override;

private:
    std::shared_ptr<Freeform> curItem{};
    QPointF m_lastPoint{};
};

#endif  // FREEFORMTOOL_H

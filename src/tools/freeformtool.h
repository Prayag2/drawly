#ifndef FREEFORMTOOL_H
#define FREEFORMTOOL_H

#include "drawingtool.h"
#include <QElapsedTimer>
class FreeformItem;
class PropertyManager;

class FreeformTool : public DrawingTool {
public:
    FreeformTool();
    ~FreeformTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void cleanup() override;

    Tool::Type type() const override;

private:
    std::shared_ptr<FreeformItem> curItem{};
    QPointF m_lastPoint{};
};

#endif  // FREEFORMTOOL_H

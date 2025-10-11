#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "tool.h"
#include "../common/constants.h"
#include <unordered_set>
class Item;
class PropertyManager;

class EraserTool : public Tool {
public:
    EraserTool(const PropertyManager& propertyManager);
    ~EraserTool() override = default;

    QString iconAlt() const override;
    void mousePressed(ApplicationContext* context) override;
    void mouseMoved(ApplicationContext* context) override;
    void mouseReleased(ApplicationContext* context) override;
    void keyPressed(ApplicationContext* context) override;
    void keyReleased(ApplicationContext* context) override;

    ToolID id() const override;

private:
    bool m_isErasing{false};
    QRectF m_lastRect{};

    std::unordered_set<std::shared_ptr<Item>> m_toBeErased;
};

#endif  // ERASERTOOL_H

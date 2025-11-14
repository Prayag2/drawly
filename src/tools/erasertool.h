#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "../common/constants.h"
#include "tool.h"
#include <unordered_set>
class Item;
class PropertyManager;

class EraserTool : public Tool {
public:
    EraserTool();
    ~EraserTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void leave(ApplicationContext *context) override;

    void cleanup() override;

    Tool::Type type() const override;

private:
    bool m_isErasing{false};
    QRectF m_lastRect{};

    std::unordered_set<std::shared_ptr<Item>> m_toBeErased;
};

#endif  // ERASERTOOL_H

#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "tool.h"
#include <unordered_set>
class Item;

class EraserTool : public Tool {
public:
    EraserTool();
    ~EraserTool() override = default;

    QString iconAlt() const override;
    void mousePressed(ApplicationContext* context) override;
    void mouseMoved(ApplicationContext* context) override;
    void mouseReleased(ApplicationContext* context) override;

private:
    bool m_isErasing{false};

    std::unordered_set<std::shared_ptr<Item>> m_toBeErased;
};

#endif  // ERASERTOOL_H

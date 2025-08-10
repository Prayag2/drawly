#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include "../tool.h"
class SelectionToolState;

class SelectionTool : public Tool {
public:
    SelectionTool();
    ~SelectionTool() = default;

    QString iconAlt() const override;

    void mousePressed(ApplicationContext* context) override;
    void mouseMoved(ApplicationContext* context) override;
    void mouseReleased(ApplicationContext* context) override;

    ToolID id() const override;

private:
    std::shared_ptr<SelectionToolState> getCurrentState(ApplicationContext* context);

    std::shared_ptr<SelectionToolState> m_moveState;
    std::shared_ptr<SelectionToolState> m_selectState;
};

#endif  // SELECTIONTOOL_H

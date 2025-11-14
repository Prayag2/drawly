#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include "../tool.h"
class SelectionToolState;

class SelectionTool : public Tool {
public:
    SelectionTool();
    ~SelectionTool() = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;

    const QVector<Property::Type> properties() const override;

    Tool::Type type() const override;

private:
    std::shared_ptr<SelectionToolState> getCurrentState(ApplicationContext *context);

    std::shared_ptr<SelectionToolState> m_moveState;
    std::shared_ptr<SelectionToolState> m_selectState;
    std::shared_ptr<SelectionToolState> m_curState;

    bool m_stateLocked{false};
};

#endif  // SELECTIONTOOL_H

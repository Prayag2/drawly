#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include "tool.h"

class SelectionTool : public Tool {
public:
    SelectionTool();
    ~SelectionTool() override = default;

    QString iconAlt() const override;

    void mousePressed(ApplicationContext* context) override;
    void mouseMoved(ApplicationContext* context) override;
    void mouseReleased(ApplicationContext* context) override;
    const bool lowFpsTolerant() const override;

protected:
    bool m_isSelecting{false};
    bool m_isMoving{false};

    QPointF m_startPoint{0, 0};
    QPointF m_lastPos{0, 0};
};

#endif  // SELECTIONTOOL_H

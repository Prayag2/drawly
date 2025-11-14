#ifndef MOVETOOL_H
#define MOVETOOL_H

#include "tool.h"
#include <QElapsedTimer>
class Item;

class MoveTool : public Tool {
public:
    MoveTool();
    ~MoveTool() = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;

    Tool::Type type() const override;

    QString tooltip() const override;
    IconManager::Icon icon() const override;

private:
    bool m_isActive{false};
    QPointF m_initialOffsetPos{};
    QPointF m_initialPos{};
};

#endif  // MOVETOOL_H

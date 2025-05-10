#ifndef MOVETOOL_H
#define MOVETOOL_H

#include "tool.h"
#include <QElapsedTimer>
class Item;

class MoveTool : public Tool {
public:
    MoveTool();
    ~MoveTool() = default;

    QString iconAlt() const override;
    void mousePressed(ApplicationContext* context) override;
    void mouseMoved(ApplicationContext* context) override;
    void mouseReleased(ApplicationContext* context) override;

private:
    bool m_isActive{false};
    QPointF m_initialOffsetPos{};
    QPointF m_initialPos{};
};

#endif  // MOVETOOL_H

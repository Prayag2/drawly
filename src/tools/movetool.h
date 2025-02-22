#ifndef MOVETOOL_H
#define MOVETOOL_H

#include "tool.h"
class Item;

class MoveTool : public Tool {
  public:
    MoveTool();
    ~MoveTool() = default;

    QString iconAlt() const override;
    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;

  private:
    bool m_isActive {false};
    QPoint m_initialOffsetPos {};
    QPoint m_initialPos {};
    QVector<std::shared_ptr<Item>> m_itemsToBeRedrawn {};
};

#endif // MOVETOOL_H

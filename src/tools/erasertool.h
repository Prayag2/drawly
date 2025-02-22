#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "tool.h"

class EraserTool : public Tool {
  public:
    EraserTool();
    ~EraserTool() override = default;

    QString iconAlt() const override;
    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;

  private:
    bool m_isErasing {false};
};

#endif // ERASERTOOL_H

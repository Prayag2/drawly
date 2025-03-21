#ifndef FREEFORMTOOL_H
#define FREEFORMTOOL_H

#include "drawingtool.h"
class Freeform;

class FreeformTool : public DrawingTool {
  public:
    FreeformTool();
    ~FreeformTool() override = default;

    QString iconAlt() const override;
    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;

  private:
    std::shared_ptr<Freeform> curItem {};
};

#endif // FREEFORMTOOL_H

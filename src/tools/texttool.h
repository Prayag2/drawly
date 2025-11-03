#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "../item/text.h"
#include "drawingtool.h"
class PropertyManager;

class TextTool : public DrawingTool {
public:
    TextTool(const PropertyManager &propertyManager);
    ~TextTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;
    void keyReleased(ApplicationContext *context) override;

    ToolID id() const override;
    QString iconAlt() const override;

private:
    std::shared_ptr<Text> m_curItem{nullptr};

    bool m_isSelecting{false};
};

#endif  // TEXTTOOL_H

#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "../item/text.h"
#include "drawingtool.h"
class PropertyManager;

class TextTool : public DrawingTool {
public:
    TextTool();
    ~TextTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;
    void keyReleased(ApplicationContext *context) override;
    void mouseDoubleClick(ApplicationContext *context) override;
    void mouseTripleClick(ApplicationContext *context) override;

    void cleanup() override;

    Tool::Type type() const override;
    QString tooltip() const override;
    IconManager::Icon icon() const override;

private:
    std::shared_ptr<TextItem> m_curItem{nullptr};

    bool m_isSelecting{false};
    bool m_mouseMoved{false};
    bool m_doubleClicked{false};
    bool m_tripleClicked{false};
};

#endif  // TEXTTOOL_H

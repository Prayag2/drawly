#ifndef DRAWINGTOOL_H
#define DRAWINGTOOL_H

#include "tool.h"
#include "../item/factory/itemfactory.h"

class DrawingTool : public Tool
{
protected:
    std::unique_ptr<ItemFactory> m_itemFactory {};

public:
    DrawingTool();
    ~DrawingTool() override = default;

protected:
    bool m_isDrawing {false};
};

#endif // DRAWINGTOOL_H

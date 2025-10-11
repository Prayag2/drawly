#ifndef TOOL_H
#define TOOL_H

#include <QCursor>
class ApplicationContext;
class ToolProperty;
enum class ToolPropertyType;

enum class ToolID {
    SelectionTool,
    FreeformTool,
    PolygonDrawingTool,
    EraserTool,
    MoveTool,
    RectangleTool,
    EllipseTool,
    LineTool,
    ArrowTool,
    TextTool
};

// INTERFACE
class Tool {
public:
    Tool();
    virtual ~Tool();

    const QCursor& cursor() const;
    virtual QString iconAlt() const = 0;

    virtual void mousePressed(ApplicationContext* context) = 0;
    virtual void mouseMoved(ApplicationContext* context) = 0;
    virtual void mouseReleased(ApplicationContext* context) = 0;
    virtual void keyPressed(ApplicationContext* context) = 0;
    virtual void keyReleased(ApplicationContext* context) = 0;

    virtual ToolID id() const = 0;

    const QVector<std::shared_ptr<ToolProperty>> properties() const;

protected:
    QCursor m_cursor{};
    std::unordered_map<ToolPropertyType, std::shared_ptr<ToolProperty>> m_properties{};
};

#endif  // TOOL_H

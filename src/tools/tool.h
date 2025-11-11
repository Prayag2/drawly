#ifndef TOOL_H
#define TOOL_H

#include "../properties/property.h"
#include <QCursor>
class ApplicationContext;

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

    const QCursor &cursor() const;
    virtual QString iconAlt() const = 0;

    virtual void mousePressed(ApplicationContext *context);
    virtual void mouseMoved(ApplicationContext *context);
    virtual void mouseReleased(ApplicationContext *context);
    virtual void keyPressed(ApplicationContext *context);
    virtual void keyReleased(ApplicationContext *context);
    virtual void mouseDoubleClick(ApplicationContext *context);
    virtual void mouseTripleClick(ApplicationContext *context);

    virtual ToolID id() const = 0;

    virtual const QVector<Property::Type> properties() const;

protected:
    QCursor m_cursor{};
    QVector<Property::Type> m_properties;
};

#endif  // TOOL_H

#ifndef TOOL_H
#define TOOL_H

#include "../iconmanager/iconmanager.h"
#include "../properties/property.h"
#include <QCursor>
class ApplicationContext;

// INTERFACE
class Tool {
public:
    Tool();
    virtual ~Tool();

    const QCursor &cursor() const;
    virtual QString tooltip() const = 0;

    virtual void mousePressed(ApplicationContext *context);
    virtual void mouseMoved(ApplicationContext *context);
    virtual void mouseReleased(ApplicationContext *context);
    virtual void keyPressed(ApplicationContext *context);
    virtual void keyReleased(ApplicationContext *context);
    virtual void mouseDoubleClick(ApplicationContext *context);
    virtual void mouseTripleClick(ApplicationContext *context);
    virtual void leave(ApplicationContext *context);

    virtual void cleanup();

    enum Type {
        Selection,
        Freeform,
        PolygonDrawing,
        Eraser,
        Move,
        Rectangle,
        Ellipse,
        Line,
        Arrow,
        Text
    };

    virtual Type type() const = 0;
    virtual IconManager::Icon icon() const = 0;

    virtual const QVector<Property::Type> properties() const;

protected:
    QCursor m_cursor{};
    QVector<Property::Type> m_properties;
};

#endif  // TOOL_H

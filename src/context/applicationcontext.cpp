#include <QRect>
#include "applicationcontext.h"
#include "../tools/rectangletool.h"
#include "../tools/ellipsetool.h"

ApplicationContext::ApplicationContext(QWidget* parent)
    : QObject {parent} {
    m_canvas = new Canvas(parent);
    m_toolbar = new Toolbar(parent);
    m_pen = new QPen();
    m_quadtree = new QuadTree(QRect(0, 0, 1000, 1000), 10);
    m_event = new Event();

    m_toolbar->addTool(new RectangleTool());
    m_toolbar->addTool(new EllipseTool());

    pen().setWidth(2);
    pen().setColor(Qt::black);
}

ApplicationContext::~ApplicationContext() {
    delete m_pen;
    delete m_quadtree;
    delete m_event;
}

Canvas& ApplicationContext::canvas() const {
    return *m_canvas;
}

QuadTree& ApplicationContext::quadtree() const {
    return *m_quadtree;
}

Toolbar& ApplicationContext::toolbar() const {
    return *m_toolbar;
}

QPen& ApplicationContext::pen() const {
    return *m_pen;
}

Event& ApplicationContext::event() const {
    return *m_event;
}

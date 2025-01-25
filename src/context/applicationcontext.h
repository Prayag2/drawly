#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QPoint>
#include <QPen>
#include "../data-structures/quadtree.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"
#include "../components/toolbar.h"
#include "../event/event.h"

class ApplicationContext : public QObject
{
public:
    ApplicationContext(QWidget *parent = nullptr);
    ~ApplicationContext();
    Canvas& canvas() const;
    QuadTree& quadtree() const;
    Toolbar& toolbar() const;
    QPen& pen() const;
    Event& event() const;

private:
    QuadTree *m_quadtree {nullptr};
    Canvas *m_canvas {nullptr};
    Toolbar *m_toolbar {nullptr};
    QPen *m_pen {nullptr};
    Event *m_event {nullptr};

    int m_scale {};
    QPoint m_centerPos {};
};

#endif // APPLICATIONCONTEXT_H

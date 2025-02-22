#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QPoint>
#include <QPen>
#include "../data-structures/quadtree.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"
#include "../components/toolbar.h"
#include "../event/event.h"

class ApplicationContext : public QObject {
  public:
    ApplicationContext(QWidget *parent = nullptr);
    ~ApplicationContext();
    Canvas& canvas() const;
    QuadTree& quadtree() const;
    Toolbar& toolbar() const;
    QPen& pen() const;
    Event& event() const;
    QPainter& canvasPainter() const;
    QPainter& overlayPainter() const;
    const QPoint& offsetPos() const;
    void setOffsetPos(const QPoint& pos);

  private slots:
    void beginPainters();
    void endPainters();
    void toolChanged(Tool&);

  private:
    QuadTree *m_quadtree {nullptr};
    Canvas *m_canvas {nullptr};
    Toolbar *m_toolbar {nullptr};
    QPen *m_pen {nullptr};
    Event *m_event {nullptr};
    QPainter *m_canvasPainter {};
    QPainter *m_overlayPainter {};

    int m_scale {};
    QPoint m_offsetPos {}; // all `Items` are positioned relative to this point
};

#endif // APPLICATIONCONTEXT_H

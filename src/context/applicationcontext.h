#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QObject>
#include <QPoint>
#include <QPen>
class ToolBar;
class Tool;
class Event;
class QuadTree;
class Canvas;
class PropertyBar;
class PropertyManager;

class ApplicationContext : public QObject {
  public:
    ApplicationContext(QWidget *parent = nullptr);
    ~ApplicationContext();
    Canvas& canvas() const;
    QuadTree& quadtree() const;
    ToolBar& toolBar() const;
    PropertyBar& propertyBar() const;
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
    ToolBar *m_toolBar {nullptr};
    PropertyBar *m_propertyBar {};
    PropertyManager *m_propertyManager {};
    QPen *m_pen {nullptr};
    Event *m_event {nullptr};
    QPainter *m_canvasPainter {};
    QPainter *m_overlayPainter {};

    int m_scale {};
    QPoint m_offsetPos {}; // all `Items` are positioned relative to this point
};

#endif // APPLICATIONCONTEXT_H

#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QObject>
#include <QPoint>
#include <QPainter>
class ToolBar;
class ActionBar;
class Tool;
class Event;
class QuadTree;
class CacheGrid;
class Canvas;
class PropertyBar;
class PropertyManager;

class ApplicationContext : public QObject {
  public:
    ApplicationContext(QWidget *parent = nullptr);
    ~ApplicationContext();

    Canvas& canvas() const;
    QuadTree& quadtree() const;
    CacheGrid& cacheGrid() const;
    ToolBar& toolBar() const;
    PropertyBar& propertyBar() const;
    ActionBar& actionBar() const;
    Event& event() const;
    QPainter& canvasPainter() const;
    QPainter& overlayPainter() const;

    int fps() const;
    const QPoint& offsetPos() const;
    void setOffsetPos(const QPoint& pos);
    qreal zoomFactor() const;
    void setZoomFactor(int diff);

  private slots:
    void beginPainters();
    void endPainters();
    void canvasResized();
    void toolChanged(Tool&);

  private:
    std::unique_ptr<QuadTree> m_quadtree {nullptr};
    std::unique_ptr<CacheGrid> m_cacheGrid{nullptr};
    Canvas *m_canvas {nullptr};
    QPainter *m_canvasPainter {};
    QPainter *m_overlayPainter {};
    ToolBar *m_toolBar {nullptr};
    PropertyBar *m_propertyBar {};
    ActionBar *m_actionBar {};
    PropertyManager *m_propertyManager {};
    Event *m_event {nullptr};

    qreal m_zoomFactor {0.8};

    int m_fps {};
    QPoint m_offsetPos {}; // all `Items` are positioned relative to this point
};

#endif // APPLICATIONCONTEXT_H

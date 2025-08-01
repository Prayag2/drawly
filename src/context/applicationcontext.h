#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QTimer>
#include <memory>
#include <unordered_set>
class ToolBar;
class ActionBar;
class Tool;
class Event;
class QuadTree;
class CacheGrid;
class Canvas;
class PropertyBar;
class PropertyManager;
class CoordinateTransformer;
class Item;

class ApplicationContext : public QObject {
public:
    ApplicationContext(QWidget* parent = nullptr);
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

    CoordinateTransformer& coordinateTransformer() const;
    std::unordered_set<std::shared_ptr<Item>>& selectedItems();
    QRectF selectionBox() const;

    void markForRender();
    void markForUpdate();
    void markForUpdate(const QRect& region);

    const QPointF& offsetPos() const;
    void setOffsetPos(const QPointF& pos);
    qreal zoomFactor() const;
    void setZoomFactor(int diff);
    const int fps() const;

private slots:
    void beginPainters();
    void endPainters();
    void canvasResized();
    void toolChanged(Tool&);

private:
    std::unique_ptr<QuadTree> m_quadtree{nullptr};
    std::unique_ptr<CacheGrid> m_cacheGrid{nullptr};
    std::unique_ptr<CoordinateTransformer> m_coordinateTransformer{nullptr};
    std::unordered_set<std::shared_ptr<Item>> m_selectedItems{};

    Canvas* m_canvas{nullptr};
    QPainter* m_canvasPainter{};
    QPainter* m_overlayPainter{};

    ToolBar* m_toolBar{nullptr};
    PropertyBar* m_propertyBar{};
    ActionBar* m_actionBar{};
    PropertyManager* m_propertyManager{};
    Event* m_event{nullptr};

    QTimer m_frameTimer;
    bool m_needsReRender{false};
    bool m_needsUpdate{false};
    QRect m_updateRegion{};

    qreal m_zoomFactor{1};

    QPointF m_offsetPos{};  // all `Items` are positioned relative to this point
};

#endif  // APPLICATIONCONTEXT_H

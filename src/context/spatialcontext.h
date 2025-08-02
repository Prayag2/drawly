#ifndef SPATIALCONTEXT_H
#define SPATIALCONTEXT_H

#include <QWidget>
class QuadTree;
class CacheGrid;
class CoordinateTransformer;
class ApplicationContext;
class Canvas;

class SpatialContext : public QObject {
public:
    SpatialContext(ApplicationContext* context);
    ~SpatialContext();

    void setSpatialContext();

    // SpatialContext
    QuadTree& quadtree() const;
    CacheGrid& cacheGrid() const;
    CoordinateTransformer& coordinateTransformer() const;
    const QPointF& offsetPos() const;
    void setOffsetPos(const QPointF& pos);

private:
    std::unique_ptr<QuadTree> m_quadtree{nullptr};
    std::unique_ptr<CacheGrid> m_cacheGrid{nullptr};
    std::unique_ptr<CoordinateTransformer> m_coordinateTransformer{nullptr};

    QPointF m_offsetPos{};  // all `Items` are positioned relative to this point

    ApplicationContext* m_applicationContext;
};

#endif  // SPATIALCONTEXT_H

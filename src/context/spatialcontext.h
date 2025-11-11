#ifndef SPATIALCONTEXT_H
#define SPATIALCONTEXT_H

#include <QWidget>
class QuadTree;
class CacheGrid;
class CoordinateTransformer;
class ApplicationContext;
class CommandHistory;
class Canvas;

class SpatialContext : public QObject {
public:
    SpatialContext(ApplicationContext *context);
    ~SpatialContext();

    void setSpatialContext();

    // SpatialContext
    QuadTree &quadtree() const;
    CacheGrid &cacheGrid() const;
    CoordinateTransformer &coordinateTransformer() const;
    CommandHistory &commandHistory() const;

    const QPointF &offsetPos() const;
    void setOffsetPos(const QPointF &pos);

private:
    std::unique_ptr<QuadTree> m_quadtree{nullptr};
    std::unique_ptr<CacheGrid> m_cacheGrid{nullptr};
    std::unique_ptr<CoordinateTransformer> m_coordinateTransformer{nullptr};
    std::unique_ptr<CommandHistory> m_commandHistory{nullptr};

    // Stores the position of the topleft corner of the viewport with respect to
    // to the world center. If viewport moves down/right, the coordinates increase
    QPointF m_offsetPos{};  

    ApplicationContext *m_applicationContext;
};

#endif  // SPATIALCONTEXT_H

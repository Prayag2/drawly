#include "spatialcontext.h"

#include "../canvas/canvas.h"
#include "../command/commandhistory.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "applicationcontext.h"
#include "coordinatetransformer.h"
#include "renderingcontext.h"
#include <memory>

SpatialContext::SpatialContext(ApplicationContext *context)
    : QObject{context},
      m_applicationContext{context} {
}

SpatialContext::~SpatialContext() {
    qDebug() << "Object deleted: SpatialContext";
}

void SpatialContext::setSpatialContext() {
    Canvas &canvas{m_applicationContext->renderingContext().canvas()};

    m_quadtree = std::make_unique<QuadTree>(QRect{{0, 0}, canvas.sizeHint()}, 100);
    m_coordinateTransformer = std::make_unique<CoordinateTransformer>(m_applicationContext);
    m_cacheGrid = std::make_unique<CacheGrid>(100);
    m_commandHistory = std::make_unique<CommandHistory>(m_applicationContext);
}

QuadTree &SpatialContext::quadtree() const {
    return *m_quadtree;
}

CacheGrid &SpatialContext::cacheGrid() const {
    return *m_cacheGrid;
}

CoordinateTransformer &SpatialContext::coordinateTransformer() const {
    return *m_coordinateTransformer;
}

CommandHistory &SpatialContext::commandHistory() const {
    return *m_commandHistory;
}

const QPointF &SpatialContext::offsetPos() const {
    return m_offsetPos;
}

void SpatialContext::setOffsetPos(const QPointF &pos) {
    m_offsetPos = pos;
}

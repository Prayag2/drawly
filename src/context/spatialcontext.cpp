#include <memory>
#include "spatialcontext.h"
#include "applicationcontext.h"
#include "renderingcontext.h"
#include "coordinatetransformer.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"

SpatialContext::SpatialContext(ApplicationContext* context)
    : QObject{context}, m_applicationContext{context} {}

void SpatialContext::setSpatialContext() {
    Canvas& canvas{m_applicationContext->renderingContext().canvas()};

    m_quadtree = std::make_unique<QuadTree>(QRect{{0, 0}, canvas.sizeHint()}, 100);
    m_coordinateTransformer = std::make_unique<CoordinateTransformer>(m_applicationContext);
    m_cacheGrid = std::make_unique<CacheGrid>(100);
}

SpatialContext::~SpatialContext() {}

QuadTree& SpatialContext::quadtree() const {
    return *m_quadtree;
}

CacheGrid& SpatialContext::cacheGrid() const {
    return *m_cacheGrid;
}

CoordinateTransformer& SpatialContext::coordinateTransformer() const {
    return *m_coordinateTransformer;
}

const QPointF& SpatialContext::offsetPos() const {
    return m_offsetPos;
}

void SpatialContext::setOffsetPos(const QPointF& pos) {
    m_offsetPos = pos;
}

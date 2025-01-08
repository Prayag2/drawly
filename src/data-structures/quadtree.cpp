#include "quadtree.h"
#include <QDebug>

QuadTree::QuadTree(QRect region, int capacity)
    : m_boundingBox {region}, m_capacity {capacity} {}

QuadTree::~QuadTree() {
    delete m_topLeft;
    delete m_topRight;
    delete m_bottomRight;
    delete m_bottomLeft;
}

void QuadTree::subdivide() {
    int x {m_boundingBox.x()};
    int y {m_boundingBox.y()};
    int halfWidth {m_boundingBox.width()/2};
    int halfHeight {m_boundingBox.height()/2};
    m_topLeft = new QuadTree{QRect{x, y, halfWidth, halfHeight}, m_capacity};
    m_topRight = new QuadTree{QRect{x+halfWidth, y, halfWidth, halfHeight}, m_capacity};
    m_bottomRight = new QuadTree{QRect{x+halfWidth, y+halfHeight, halfWidth, halfHeight}, m_capacity};
    m_bottomLeft = new QuadTree{QRect{x, y+halfHeight, halfWidth, halfHeight}, m_capacity};
}

void QuadTree::insertShape(Shape* const shape) {
    /*
     * TODO: If the shape lies outside this node's bounding box,
     *       then, expand its bounding box. This will make the
     *       whiteboard infinite.
     */
    // For now, the whiteboard is finite:
    insert(shape);
}

bool QuadTree::insert(Shape* const shape) {
    if (!m_boundingBox.intersects(shape->boundingBox())) {
        return false;
    }

    if (m_shapes.size() < m_capacity) {
        m_shapes.push_back(shape);
        return true;
    }

    // subdivide if not already subdivided
    if (m_topLeft == nullptr) subdivide();

    bool inserted = false;
    if (m_topLeft->insert(shape)) inserted = true;
    if (m_topRight->insert(shape)) inserted = true;
    if (m_bottomRight->insert(shape)) inserted = true;
    if (m_bottomLeft->insert(shape)) inserted = true;

    return inserted;
}

void QuadTree::deleteShape(Shape* const shape) {
    if (!m_boundingBox.intersects(shape->boundingBox())) {
        return;
    }

    auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);
    if (it != m_shapes.end()) {
        m_shapes.erase(it);
    }

    // If the node is subdivided, attempt to delete the shape from children
    if (m_topLeft != nullptr) {
        m_topLeft->deleteShape(shape);
        m_topRight->deleteShape(shape);
        m_bottomLeft->deleteShape(shape);
        m_bottomRight->deleteShape(shape);
    }
}

void QuadTree::deleteShapes(const QRect& boundingBox) {
    if (!m_boundingBox.intersects(boundingBox)) return;

    for (int i = 0; i < m_shapes.size();) {
        if (boundingBox.intersects(m_shapes[i]->boundingBox())) {
            m_shapes.erase(m_shapes.begin()+i);
        } else {
            i++;
        }
    }

    if (m_topLeft != nullptr) {
        m_topLeft->deleteShapes(boundingBox);
        m_topRight->deleteShapes(boundingBox);
        m_bottomRight->deleteShapes(boundingBox);
        m_bottomLeft->deleteShapes(boundingBox);
    }
}

QVector<Shape*> QuadTree::queryShapes(const QRect& boundingBox, bool onlyBoundingBox) const {
    QVector<Shape*> curShapes {};

    if (!m_boundingBox.intersects(boundingBox)) {
        return curShapes;
    }

    for (Shape* const shape : m_shapes) {
        if (shape->boundingBox().intersects(boundingBox)) {
            if (onlyBoundingBox || shape->intersects(boundingBox)) {
                curShapes.push_back(shape);
            }
        }
    }

    // if this node has sub-regions
    if (m_topLeft != nullptr) {
        curShapes += m_topLeft->queryShapes(boundingBox);
        curShapes += m_topRight->queryShapes(boundingBox);
        curShapes += m_bottomRight->queryShapes(boundingBox);
        curShapes += m_bottomLeft->queryShapes(boundingBox);
    }
    return curShapes;
};

QVector<Shape*> QuadTree::queryAllShapes(const QRect& boundingBox, std::optional<int> level) const {
    // Performs DFS
    // Creates a list of all the shapes intersecting directly or indirectly
    // with the given bounding box
    QVector<Shape*> output {};
    dfs(boundingBox, m_shapes, output, level);
    return output;
};

void QuadTree::dfs(const QRect& boundingBox, QVector<Shape*> shapes, QVector<Shape*>& out, std::optional<int> level) const {
    if (level.has_value() && level.value() <= 0) return;
    if (!m_boundingBox.intersects(boundingBox)) {
        return;
    }

    std::optional<int> nextLevel {level.has_value() ? level.value()-1 : level};
    int N = shapes.size();
    for (int i = 0; i < N; i++) {
        if (shapes[i] == nullptr) continue;
        if (shapes[i]->boundingBox().intersects(boundingBox)) {
            out.push_back(shapes[i]);
            QRect newBox = shapes[i]->boundingBox();
            shapes[i] = nullptr;
            dfs(newBox, shapes, out, nextLevel);
        }
    }

    // if this node has sub-regions
    if (m_topLeft != nullptr) {
        m_topLeft->dfs(boundingBox, m_topLeft->m_shapes, out, nextLevel);
        m_topRight->dfs(boundingBox, m_topRight->m_shapes, out, nextLevel);
        m_bottomRight->dfs(boundingBox, m_bottomRight->m_shapes, out, nextLevel);
        m_bottomLeft->dfs(boundingBox, m_bottomLeft->m_shapes, out, nextLevel);
    }
};


int QuadTree::size() const {
    if (m_topLeft == nullptr) return 1;

    int totalNodes {1};
    totalNodes += m_topLeft->size();
    totalNodes += m_topRight->size();
    totalNodes += m_bottomRight->size();
    totalNodes += m_bottomLeft->size();
    return totalNodes;
}

void QuadTree::draw(QPainter& painter) const {
    painter.drawRect(m_boundingBox);

    if (m_topLeft != nullptr) {
        m_topLeft->draw(painter);
        m_topRight->draw(painter);
        m_bottomRight->draw(painter);
        m_bottomLeft->draw(painter);
    }
}

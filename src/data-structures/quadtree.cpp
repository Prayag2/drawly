#include <QDebug>
#include "quadtree.h"

QuadTree::QuadTree(QRect region, int capacity)
    : m_boundingBox {region}, m_capacity {capacity} {}

QuadTree::~QuadTree() {}

void QuadTree::subdivide() {
    int x {m_boundingBox.x()};
    int y {m_boundingBox.y()};
    int halfWidth {m_boundingBox.width()/2};
    int halfHeight {m_boundingBox.height()/2};
    m_topLeft = std::make_unique<QuadTree>(QRect{x, y, halfWidth, halfHeight}, m_capacity);
    m_topRight = std::make_unique<QuadTree>(QRect{x+halfWidth, y, halfWidth, halfHeight}, m_capacity);
    m_bottomRight = std::make_unique<QuadTree>(QRect{x+halfWidth, y+halfHeight, halfWidth, halfHeight}, m_capacity);
    m_bottomLeft = std::make_unique<QuadTree>(QRect{x, y+halfHeight, halfWidth, halfHeight}, m_capacity);
}

void QuadTree::insertItem(std::shared_ptr<Item> item) {
    /*
     * TODO: If the item lies outside this node's bounding box,
     *       then, expand its bounding box. This will make the
     *       whiteboard infinite.
     */
    // For now, the whiteboard is finite:
    insert(item);
}

bool QuadTree::insert(std::shared_ptr<Item> item) {
    if (!m_boundingBox.intersects(item->boundingBox())) {
        return false;
    }

    if (m_items.size() < m_capacity) {
        m_items.push_back(item);
        return true;
    }

    // subdivide if not already subdivided
    if (m_topLeft == nullptr) subdivide();

    bool inserted = false;
    if (m_topLeft->insert(item)) inserted = true;
    if (m_topRight->insert(item)) inserted = true;
    if (m_bottomRight->insert(item)) inserted = true;
    if (m_bottomLeft->insert(item)) inserted = true;

    return inserted;
}

void QuadTree::deleteItem(std::shared_ptr<Item> const item) {
    if (!m_boundingBox.intersects(item->boundingBox())) {
        return;
    }

    auto it = std::find(m_items.begin(), m_items.end(), item);
    if (it != m_items.end()) {
        m_items.erase(it);
    }

    // If the node is subdivided, attempt to delete the item from children
    if (m_topLeft != nullptr) {
        m_topLeft->deleteItem(item);
        m_topRight->deleteItem(item);
        m_bottomLeft->deleteItem(item);
        m_bottomRight->deleteItem(item);
    }
}

void QuadTree::deleteItems(const QRect& boundingBox) {
    if (!m_boundingBox.intersects(boundingBox)) return;

    for (int i = 0; i < m_items.size();) {
        if (boundingBox.intersects(m_items[i]->boundingBox())) {
            m_items.erase(m_items.begin()+i);
        } else {
            i++;
        }
    }

    if (m_topLeft != nullptr) {
        m_topLeft->deleteItems(boundingBox);
        m_topRight->deleteItems(boundingBox);
        m_bottomRight->deleteItems(boundingBox);
        m_bottomLeft->deleteItems(boundingBox);
    }
}

QVector<std::shared_ptr<Item>> QuadTree::getAllItems() const {
    QVector<std::shared_ptr<Item>> curItems {m_items};
    if (m_topLeft != nullptr) {
        curItems += m_topLeft->getAllItems();
        curItems += m_topRight->getAllItems();
        curItems += m_bottomRight->getAllItems();
        curItems += m_bottomLeft->getAllItems();
    }
    return curItems;
}

QVector<std::shared_ptr<Item>> QuadTree::queryItems(const QRect& boundingBox, bool onlyBoundingBox) const {
    QVector<std::shared_ptr<Item>> curItems {};
    query(boundingBox, onlyBoundingBox, curItems);
    return curItems;
};

void QuadTree::query(const QRect& boundingBox, bool onlyBoundingBox, QVector<std::shared_ptr<Item>>& out) const {
    if (!m_boundingBox.intersects(boundingBox)) {
        return;
    }

    for (const std::shared_ptr<Item>& item : m_items) {
        if (
            item->boundingBox().intersects(boundingBox)
        ) {
            if (onlyBoundingBox || item->intersects(boundingBox)) {
                out.push_back(item);
            }
        }
    }

    // if this node has sub-regions
    if (m_topLeft != nullptr) {
        m_topLeft->query(boundingBox, onlyBoundingBox, out);
        m_topRight->query(boundingBox, onlyBoundingBox, out);
        m_bottomRight->query(boundingBox, onlyBoundingBox, out);
        m_bottomLeft->query(boundingBox, onlyBoundingBox, out);
    }
}

QVector<std::shared_ptr<Item>> QuadTree::queryConnectedItems(const QRect& boundingBox, std::optional<int> level) const {
    // Performs DFS
    // Creates a list of all the items intersecting directly or indirectly
    // with the given bounding box
    QVector<std::shared_ptr<Item>> output {};
    dfs(boundingBox, m_items, output, level);
    return output;
};

void QuadTree::dfs(const QRect& boundingBox, QVector<std::shared_ptr<Item>> items, QVector<std::shared_ptr<Item>>& out, std::optional<int> level) const {
    if (level.has_value() && level.value() <= 0) return;
    if (!m_boundingBox.intersects(boundingBox)) {
        return;
    }

    std::optional<int> nextLevel {level.has_value() ? level.value()-1 : level};
    int N = items.size();
    for (int i = 0; i < N; i++) {
        if (items[i] == nullptr) continue;
        if (items[i]->boundingBox().intersects(boundingBox)) {
            out.push_back(items[i]);
            QRect newBox = items[i]->boundingBox();
            items[i] = nullptr;
            dfs(newBox, items, out, nextLevel);
        }
    }

    // if this node has sub-regions
    if (m_topLeft != nullptr) {
        m_topLeft->dfs(boundingBox, m_topLeft->m_items, out, nextLevel);
        m_topRight->dfs(boundingBox, m_topRight->m_items, out, nextLevel);
        m_bottomRight->dfs(boundingBox, m_bottomRight->m_items, out, nextLevel);
        m_bottomLeft->dfs(boundingBox, m_bottomLeft->m_items, out, nextLevel);
    }
};

int QuadTree::size() const {
    int totalNodes {static_cast<int>(m_items.size())};

    if (m_topLeft != nullptr) {
        totalNodes += m_topLeft->size();
        totalNodes += m_topRight->size();
        totalNodes += m_bottomRight->size();
        totalNodes += m_bottomLeft->size();
    }
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

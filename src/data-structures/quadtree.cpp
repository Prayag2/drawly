#include <QDebug>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include "orderedlist.h"
#include "quadtree.h"
#include "../item/item.h"

QuadTree::QuadTree(QRectF region, int capacity)
    : m_boundingBox {region}, m_capacity {capacity} {
        if (m_orderedList == nullptr) {
            m_orderedList = std::make_shared<OrderedList>();
        }
    }

QuadTree::QuadTree(QRectF region, int capacity, std::shared_ptr<OrderedList> orderedList)
    : m_boundingBox {region}, m_capacity {capacity} {
        m_orderedList = orderedList;
    }

QuadTree::~QuadTree() {}

void QuadTree::subdivide() {
    double x {m_boundingBox.x()};
    double y {m_boundingBox.y()};
    double halfWidth {m_boundingBox.width()/2};
    double halfHeight {m_boundingBox.height()/2};

    QRectF topLeftRect {x, y, halfWidth, halfHeight};
    QRectF topRightRect {x+halfWidth, y, halfWidth, halfHeight};
    QRectF bottomRightRect {x+halfWidth, y+halfHeight, halfWidth, halfHeight};
    QRectF bottomLeftRect {x, y+halfHeight, halfWidth, halfHeight};

    m_topLeft = std::make_unique<QuadTree>(topLeftRect, m_capacity, m_orderedList);
    m_topRight = std::make_unique<QuadTree>(topRightRect, m_capacity, m_orderedList);
    m_bottomRight = std::make_unique<QuadTree>(bottomRightRect, m_capacity, m_orderedList);
    m_bottomLeft = std::make_unique<QuadTree>(bottomLeftRect, m_capacity, m_orderedList);
}

void QuadTree::insertItem(std::shared_ptr<Item> item) {
    expand(item->boundingBox().topLeft());
    expand(item->boundingBox().topRight());
    expand(item->boundingBox().bottomRight());
    expand(item->boundingBox().bottomLeft());
    insert(item);
}

bool QuadTree::insert(std::shared_ptr<Item> item) {
    if (!m_boundingBox.intersects(item->boundingBox())) {
        return false;
    }

    if (m_items.size() < m_capacity) {
        m_items.push_back(item);
        m_orderedList->insert(item);
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
        m_orderedList->remove(item);
        return;
    }

    // If the node is subdivided, attempt to delete the item from children
    if (m_topLeft != nullptr) {
        m_topLeft->deleteItem(item);
        m_topRight->deleteItem(item);
        m_bottomLeft->deleteItem(item);
        m_bottomRight->deleteItem(item);
    }
}

void QuadTree::deleteItems(const QRectF& boundingBox) {
    if (!m_boundingBox.intersects(boundingBox)) return;

    for (int i = 0; i < m_items.size();) {
        if (boundingBox.intersects(m_items[i]->boundingBox())) {
            m_items.erase(m_items.begin()+i);
            m_orderedList->remove(m_items[i]);
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

QVector<std::shared_ptr<Item>> QuadTree::queryItems(const QRectF& boundingBox, bool onlyBoundingBox) const {
    QVector<std::shared_ptr<Item>> curItems {};
    std::unordered_map<std::shared_ptr<Item>, bool> itemAlreadyPushed {};

    // look for matches and store the result in curItems
    query(boundingBox, onlyBoundingBox, curItems, itemAlreadyPushed);

    // sort based on z-index
    std::sort(curItems.begin(), curItems.end(), [&](auto& firstItem, auto& secondItem){
        return m_orderedList->zIndex(firstItem) < m_orderedList->zIndex(secondItem);
    });

    return curItems;
};

void QuadTree::query(
    const QRectF& boundingBox,
    bool onlyBoundingBox,
    QVector<std::shared_ptr<Item>>& out,
    std::unordered_map<std::shared_ptr<Item>, bool>& itemAlreadyPushed
) const {
    if (!m_boundingBox.intersects(boundingBox)) {
        return;
    }

    for (const std::shared_ptr<Item>& item : m_items) {
        if (item->boundingBox().intersects(boundingBox)) {
            if (onlyBoundingBox || item->intersects(boundingBox)) {
                // using the hash map because multiple nodes may have a pointer to the same item
                if (!itemAlreadyPushed[item]) {
                    out.push_back(item);
                    itemAlreadyPushed[item] = true;
                }
            }
        }
    }

    // if this node has sub-regions
    if (m_topLeft != nullptr) {
        m_topLeft->query(boundingBox, onlyBoundingBox, out, itemAlreadyPushed);
        m_topRight->query(boundingBox, onlyBoundingBox, out, itemAlreadyPushed);
        m_bottomRight->query(boundingBox, onlyBoundingBox, out, itemAlreadyPushed);
        m_bottomLeft->query(boundingBox, onlyBoundingBox, out, itemAlreadyPushed);
    }
}

QVector<std::shared_ptr<Item>> QuadTree::queryConnectedItems(const QRectF& boundingBox, std::optional<int> level) const {
    // Performs DFS
    // Creates a list of all the items intersecting directly or indirectly
    // with the given bounding box
    QVector<std::shared_ptr<Item>> output {};
    dfs(boundingBox, m_items, output, level);
    return output;
};

const QRectF& QuadTree::boundingBox() const {
    return m_boundingBox;
};

void QuadTree::dfs(const QRectF& boundingBox, QVector<std::shared_ptr<Item>> items, QVector<std::shared_ptr<Item>>& out, std::optional<int> level) const {
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
            QRectF newBox = items[i]->boundingBox();
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

void QuadTree::draw(QPainter& painter, const QPointF& offset) const {
    painter.drawRect(m_boundingBox.translated(offset));

    if (m_topLeft != nullptr) {
        m_topLeft->draw(painter, offset);
        m_topRight->draw(painter, offset);
        m_bottomRight->draw(painter, offset);
        m_bottomLeft->draw(painter, offset);
    }
}

void QuadTree::expand(const QPointF& point) {
    // This function grows the quadtree in size recursively if the
    // point lies outside of it, making it almost (until integer overflow) infinite!
    if (m_boundingBox.contains(point)) return;

    double treeW {m_boundingBox.width()}, treeH {m_boundingBox.height()};
    double x {point.x()}, y {point.y()};
    QPointF tl {m_boundingBox.topLeft()}, tr {m_boundingBox.topRight()};
    QPointF bl {m_boundingBox.bottomLeft()}, br {m_boundingBox.bottomRight()};

    std::unique_ptr<QuadTree> topLeft {std::make_unique<QuadTree>(m_boundingBox, m_capacity, m_orderedList)};
    std::unique_ptr<QuadTree> topRight {std::make_unique<QuadTree>(m_boundingBox, m_capacity, m_orderedList)};
    std::unique_ptr<QuadTree> bottomRight {std::make_unique<QuadTree>(m_boundingBox, m_capacity, m_orderedList)};
    std::unique_ptr<QuadTree> bottomLeft {std::make_unique<QuadTree>(m_boundingBox, m_capacity, m_orderedList)};
    std::unique_ptr<QuadTree> cur {};

    if (x < tl.x() || y < tl.y()) {
        m_boundingBox.adjust(-treeW, -treeH, 0, 0);

        topLeft->m_boundingBox.translate(-treeW, -treeH);
        topRight->m_boundingBox.translate(0, -treeH);
        bottomLeft->m_boundingBox.translate(-treeW, 0);

        cur = std::move(bottomRight);
    } else if (x > br.x() || y > br.y()) {
        m_boundingBox.adjust(0, 0, treeW, treeH);

        topRight->m_boundingBox.translate(treeW, 0);
        bottomRight->m_boundingBox.translate(treeW, treeH);
        bottomLeft->m_boundingBox.translate(0, treeH);

        cur = std::move(topLeft);
    }

    if (m_topLeft) {
        cur->m_topLeft = std::move(m_topLeft);
        cur->m_topRight = std::move(m_topRight);
        cur->m_bottomRight = std::move(m_bottomRight);
        cur->m_bottomLeft = std::move(m_bottomLeft);
    }
    cur->m_items = std::move(m_items);

    if (topLeft == nullptr) topLeft = std::move(cur);
    else if (bottomRight == nullptr) bottomRight = std::move(cur);

    m_topLeft = std::move(topLeft);
    m_topRight = std::move(topRight);
    m_bottomRight = std::move(bottomRight);
    m_bottomLeft = std::move(bottomLeft);

    // expand recursively
    expand(point);
}

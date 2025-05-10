#ifndef QUADTREE_H
#define QUADTREE_H

#include <QPainter>
#include <QRectF>
#include <QVector>
#include <memory>
#include <unordered_map>

class OrderedList;
class Item;

/*
 * A QuadTree is a tree data structure which divides the space,
 * or in our case, the canvas, into 4 regions recursively.
 * It will be useful here to detect collisions with existing items
 * And redraw only the items necessary which improves performance.
 */
/*
 * NOTE: This is tightly coupled with the OrderedList data structure present in the same directory
 *       and the Item class present in the `item` directory.
 */
class QuadTree {
public:
    using ItemPtr = std::shared_ptr<Item>;

private:
    QVector<ItemPtr> m_items{};
    QRectF m_boundingBox{};
    int m_capacity{};
    std::unique_ptr<QuadTree> m_topLeft{nullptr};
    std::unique_ptr<QuadTree> m_topRight{nullptr};
    std::unique_ptr<QuadTree> m_bottomRight{nullptr};
    std::unique_ptr<QuadTree> m_bottomLeft{nullptr};
    std::shared_ptr<OrderedList> m_orderedList{nullptr};

public:
    QuadTree(QRectF region, int capacity);
    QuadTree(QRectF region, int capacity, std::shared_ptr<OrderedList> orderedList);

    ~QuadTree();

    int size() const;
    void insertItem(ItemPtr item);
    void deleteItem(ItemPtr item);
    void deleteItems(const QRectF& boundingBox);

    QVector<ItemPtr> getAllItems() const;
    QVector<ItemPtr> queryItems(const QRectF& boundingBox, bool onlyBoundingBox = false) const;
    QVector<ItemPtr> queryConnectedItems(const QRectF& boundingBox,
                                         std::optional<int> level = {}) const;

    void draw(QPainter& painter, const QPointF& offset) const;
    const QRectF& boundingBox() const;

private:
    bool insert(ItemPtr item);
    void query(const QRectF& boundingBox, bool onlyBoundingBox, QVector<ItemPtr>& out,
               std::unordered_map<ItemPtr, bool>& itemAlreadyPushed) const;
    void dfs(const QRectF& boundingBox, QVector<ItemPtr> items, QVector<ItemPtr>& out,
             std::optional<int> level) const;
    void subdivide();
    void expand(const QPointF& point);
};

#endif  // QUADTREE_H

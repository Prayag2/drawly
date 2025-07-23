#ifndef QUADTREE_H
#define QUADTREE_H

#include "../item/item.h"
#include <QPainter>
#include <QRectF>
#include <QVector>
#include <memory>
#include <unordered_map>

class OrderedList;

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
    void updateItem(ItemPtr item, const QRectF& oldBoundingBox);
    void deleteItems(const QRectF& boundingBox);

    QVector<ItemPtr> getAllItems() const;

    template <typename Shape, typename QueryCondition>
    QVector<ItemPtr> queryItems(const Shape& shape, QueryCondition condition) const;

    template <typename Shape>
    QVector<ItemPtr> queryItems(const Shape& shape) const;

    void draw(QPainter& painter, const QPointF& offset) const;
    const QRectF& boundingBox() const;

private:
    bool insert(ItemPtr item);
    void update(ItemPtr item, const QRectF& oldBoundingBox, bool inserted);

    template <typename Shape, typename QueryCondition>
    void query(const Shape& shape, QueryCondition condition, QVector<ItemPtr>& out,
               std::unordered_map<ItemPtr, bool>& itemAlreadyPushed) const;

    void subdivide();
    void expand(const QPointF& point);
};

#include "quadtree.ipp"

#endif  // QUADTREE_H

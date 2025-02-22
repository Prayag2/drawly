#ifndef QUADTREE_H
#define QUADTREE_H

#include <QVector>
#include <QRect>
#include <QPainter>
#include "../item/item.h"

/*
 * A QuadTree is a tree data structure which divides the space,
 * or in our case, the canvas, into 4 regions recursively.
 * It will be useful here to detect collisions with existing items
 * And redraw only the items necessary which improves performance.
 * The following implementation of QuadTree is specific to this whiteboard
 * app since it uses the Item class which is implemented in the items folder.
 */
class QuadTree {
  private:
    QVector<std::shared_ptr<Item>> m_items {};
    QRect m_boundingBox {};
    int m_capacity {};
    std::unique_ptr<QuadTree> m_topLeft {nullptr};
    std::unique_ptr<QuadTree> m_topRight {nullptr};
    std::unique_ptr<QuadTree> m_bottomRight {nullptr};
    std::unique_ptr<QuadTree> m_bottomLeft {nullptr};
  public:
    QuadTree(QRect region, int capacity);
    ~QuadTree();

    int size() const;
    void insertItem(std::shared_ptr<Item> item);
    void deleteItem(std::shared_ptr<Item> item);
    void deleteItems(const QRect& boundingBox);
    QVector<std::shared_ptr<Item>> getAllItems() const;
    QVector<std::shared_ptr<Item>> queryItems(const QRect& boundingBox, bool onlyBoundingBox = false) const;
    QVector<std::shared_ptr<Item>> queryConnectedItems(const QRect& boundingBox, std::optional<int> level = {}) const;

    void draw(QPainter& painter, const QPoint& offset) const;
    const QRect& boundingBox() const;

  private:
    bool insert(std::shared_ptr<Item> item);
    void query(const QRect& boundingBox, bool onlyBoundingBox, QVector<std::shared_ptr<Item>>& out) const;
    void dfs(const QRect& boundingBox, QVector<std::shared_ptr<Item>> items, QVector<std::shared_ptr<Item>>& out, std::optional<int> level) const;
    void subdivide();
    void expand(const QPoint& point);
};

#endif // QUADTREE_H

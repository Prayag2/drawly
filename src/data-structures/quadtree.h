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
class QuadTree
{
private:
    QVector<Item*> m_items {};
    QRect m_boundingBox {};
    int m_capacity {};
    QuadTree* m_topLeft {nullptr};
    QuadTree* m_topRight {nullptr};
    QuadTree* m_bottomRight {nullptr};
    QuadTree* m_bottomLeft {nullptr};
public:
    QuadTree(QRect region, int capacity);
    ~QuadTree();

    int size() const;
    void subdivide();
    void insertItem(Item* const item);
    void deleteItem(Item* const item);
    void deleteItems(const QRect& boundingBox);
    QVector<Item*> getAllItems() const;
    QVector<Item*> queryItems(const QRect& boundingBox, bool onlyBoundingBox = false) const;
    QVector<Item*> queryConnectedItems(const QRect& boundingBox, std::optional<int> level = {}) const;

    void draw(QPainter& painter) const;

private:
    bool insert(Item* const item);
    QImage* m_img;

    void dfs(const QRect& boundingBox, QVector<Item*> items, QVector<Item*>& out, std::optional<int> level) const;
};

#endif // QUADTREE_H

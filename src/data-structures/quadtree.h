#ifndef QUADTREE_H
#define QUADTREE_H

#include <QVector>
#include <QRect>
#include "../shapes/shape.h"

/*
 * A QuadTree is a tree data structure which divides the space,
 * or in our case, the canvas, into 4 regions recursively.
 * It will be useful here to detect collisions with existing shapes
 * And redraw only the shapes necessary which improves performance.
 * The following implementation of QuadTree is specific to this whiteboard
 * app since it uses the Shape class which is implemented in the shapes folder.
 */
class QuadTree
{
private:
    QVector<Shape*> m_shapes {};
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
    void insertShape(Shape* const shape);
    void deleteShape(Shape* const shape);
    void deleteShapes(const QRect& boundingBox);
    QVector<Shape*> queryShapes(const QRect& boundingBox, bool onlyBoundingBox = false) const;
    QVector<Shape*> queryAllShapes(const QRect& boundingBox, std::optional<int> level = {}) const;

    void draw(QPainter& painter) const;

private:
    bool insert(Shape* const shape);
    QImage* m_img;

    void dfs(const QRect& boundingBox, QVector<Shape*> shapes, QVector<Shape*>& out, std::optional<int> level) const;
};

#endif // QUADTREE_H

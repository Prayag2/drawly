#ifndef ITEM_H
#define ITEM_H

#include "properties/itemproperty.h"
#include <QPainter>
#include <QRect>
class ApplicationContext;

class Item {
public:
    Item();
    virtual ~Item();

    virtual bool intersects(const QRectF& rect) = 0;
    virtual void draw(QPainter& painter, const QPointF& offset) = 0;
    virtual void erase(QPainter& painter, const QPointF& offset) const = 0;
    const QRectF& boundingBox() const;

    void setBoundingBoxPadding(int padding);
    void setScale(qreal scale);

    ItemProperty& getProperty(const ItemPropertyType propertyType);
    const ItemProperty& getProperty(const ItemPropertyType propertyType) const;

protected:
    QRectF m_boundingBox{};
    int m_boundingBoxPadding{};
    qreal m_scale{1};
    std::unordered_map<ItemPropertyType, ItemProperty> m_properties{};

    static bool linesIntersect(QLineF a, QLineF b);
    static int orientation(QPointF a, QPointF b, QPointF c);

    virtual void m_draw(QPainter& painter, const QPointF& offset) const = 0;
};

#endif  // ITEM_H

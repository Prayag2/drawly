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
    virtual bool intersects(const QLineF& rect) = 0;

    virtual void draw(QPainter& painter, const QPointF& offset) = 0;
    virtual void erase(QPainter& painter, const QPointF& offset,
                       QColor color = Qt::transparent) const = 0;

    virtual void translate(const QPointF& amount) = 0;

    const QRectF& boundingBox() const;

    void setBoundingBoxPadding(int padding);

    ItemProperty& getProperty(const ItemPropertyType propertyType);
    const ItemProperty& getProperty(const ItemPropertyType propertyType) const;

protected:
    QRectF m_boundingBox{};
    int m_boundingBoxPadding{};
    std::unordered_map<ItemPropertyType, ItemProperty> m_properties{};

    virtual void m_draw(QPainter& painter, const QPointF& offset) const = 0;
};

#endif  // ITEM_H

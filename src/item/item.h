#ifndef ITEM_H
#define ITEM_H

#include <QRect>
#include <QPainter>
#include "properties/itemproperty.h"
class ApplicationContext;

class Item {
  public:
    Item();
    virtual ~Item() = default;

    virtual bool intersects(const QRect& rect) = 0;
    virtual void draw(QPainter& painter, const QPoint& offset) const = 0;
    virtual void erase(QPainter& painter, const QPoint& offset) const = 0;
    const QRect& boundingBox() const;

    void setBoundingBoxPadding(int padding);
    void setScale(qreal scale);

    ItemProperty& getProperty(const ItemPropertyType propertyType);
    const ItemProperty& getProperty(const ItemPropertyType propertyType) const;

  protected:
    QRect m_boundingBox {};
    int m_boundingBoxPadding {};
    qreal m_scale {1};
    std::unordered_map<ItemPropertyType, ItemProperty> m_properties {};

    static bool linesIntersect(QLine a, QLine b);
    static int orientation(QPoint a, QPoint b, QPoint c);

    virtual void m_draw(QPainter& painter, const QPoint& offset) const = 0;
};

#endif // ITEM_H

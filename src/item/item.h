#ifndef ITEM_H
#define ITEM_H

#include "../properties/property.h"
#include <QPainter>
#include <QRect>

class Item {
public:
    Item();
    virtual ~Item();

    virtual bool intersects(const QRectF &rect) = 0;
    virtual bool intersects(const QLineF &rect) = 0;

    virtual void draw(QPainter &painter, const QPointF &offset) = 0;
    virtual void erase(QPainter &painter,
                       const QPointF &offset,
                       QColor color = Qt::transparent) const = 0;

    virtual void translate(const QPointF &amount) = 0;

    const QRectF boundingBox() const;

    void setBoundingBoxPadding(int padding);

    const QVector<Property::Type> properties() const;

    void setProperty(const Property::Type propertyType, Property newObj);
    const Property &property(const Property::Type propertyType) const;

    enum Type { Freeform, Rectangle, Ellipse, Line, Arrow, Text };

    virtual Type type() const = 0;

    virtual void updateAfterProperty();

protected:
    QRectF m_boundingBox{};
    int m_boundingBoxPadding{};
    std::unordered_map<Property::Type, Property> m_properties{};

    virtual void m_draw(QPainter &painter, const QPointF &offset) const = 0;
};

#endif  // ITEM_H

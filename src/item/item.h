#ifndef ITEM_H
#define ITEM_H

#include <QRect>
#include <QPainter>
#include "properties/fill.h"
#include "properties/stroke.h"
#include "properties/font.h"
class ApplicationContext;

class Item
{
public:
    Item();
    virtual ~Item() = default;

    virtual bool intersects(const QRect& rect) = 0;
    virtual void draw(QPainter& painter) const = 0;
    const QRect& boundingBox() const;

    Stroke& stroke() const;
    Fill& fill() const;
    Font& font() const;

    void setBoundingBoxPadding(int padding);

protected:
    QRect m_boundingBox {};
    int m_boundingBoxPadding {};

private:
    std::unique_ptr<Stroke> m_stroke {};
    std::unique_ptr<Fill> m_fill {};
    std::unique_ptr<Font> m_font {};
};

#endif // ITEM_H

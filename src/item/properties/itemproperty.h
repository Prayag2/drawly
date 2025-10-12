#ifndef ITEMPROPERTY_H
#define ITEMPROPERTY_H

#include <QVariant>

class ItemProperty {
public:
    ItemProperty();
    ItemProperty(QVariant value);

    void setValue(QVariant value);
    const QVariant& value() const;

    enum Type {
        StrokeWidth = 0x00,
        StrokeColor = 0x02,
        Opacity = 0x04,
        FontSize = 0x06,
    };

private:
    QVariant m_value{};
};

#endif  // ITEMPROPERTY_H

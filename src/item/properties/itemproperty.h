#ifndef ITEMPROPERTY_H
#define ITEMPROPERTY_H

#include <QVariant>

enum class ItemPropertyType {
    StrokeWidth = 0x00,
    StrokeColor = 0x02,
    Opacity = 0x04,
};

class ItemProperty {
public:
    ItemProperty();
    ItemProperty(QVariant value);

    void setValue(QVariant value);
    const QVariant& value() const;

private:
    QVariant m_value{};
};

#endif  // ITEMPROPERTY_H

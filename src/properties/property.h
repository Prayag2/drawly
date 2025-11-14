#ifndef PROPERTY_H
#define PROPERTY_H

#include <QVariant>

class Property {
public:
    Property() = default;

    enum Type { StrokeWidth, StrokeColor, Opacity, FontSize, EraserSize };

    template <typename T>
    Property(T value, Type type) {
        m_value = value;
        m_type = type;
    }

    template <typename T>
    void setValue(T value) {
        m_value = value;
    }

    template <typename T>
    const T value() const {
        return m_value.value<T>();
    }

    Type type() const;
    const QVariant variant() const;

private:
    QVariant m_value{};
    Type m_type{};
};

#endif  // PROPERTY_H

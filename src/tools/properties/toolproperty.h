#ifndef TOOLPROPERTY_H
#define TOOLPROPERTY_H

#include <QWidget>
class ItemProperty;

class ToolProperty {
public:
    ToolProperty();
    virtual QString name() const = 0;
    virtual const QVariant value() const = 0;

    QWidget *widget() const;

    enum Type { StrokeWidth, StrokeColor, EraserSize, FontSize };

protected:
    QWidget *m_widget{};
};

#endif  // TOOLPROPERTY_H

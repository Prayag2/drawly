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

protected:
    QWidget *m_widget{};
};

#endif  // TOOLPROPERTY_H

#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>
class Property;

class PropertyWidget : public QObject {
Q_OBJECT
public:
    PropertyWidget(QWidget *parent = nullptr);

    virtual QString name() const = 0;
    virtual const Property value() const = 0;

    QWidget *widget() const;

signals:
    void changed(Property property);

protected:
    QWidget *m_widget{};
};

#endif  // PROPERTYWIDGET_H

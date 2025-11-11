#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H
#include <QObject>
#include "propertywidget.h"
#include "../property.h"

class PropertyManager : public QObject {
Q_OBJECT
public:
    explicit PropertyManager(QWidget *parent = nullptr);

    const PropertyWidget& widget(const Property::Type type) const;
    Property value(const Property::Type type) const;

signals:
    void propertyUpdated(Property property);

private:
    std::unordered_map<Property::Type, PropertyWidget*> m_widgets{};
};

#endif  // PROPERTYMANAGER_H

#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H
#include <QObject>
#include "toolproperty.h"

class PropertyManager : public QObject {
public:
    explicit PropertyManager(QWidget *parent = nullptr);
    std::shared_ptr<ToolProperty> get(const ToolProperty::Type toolType) const;

private:
    std::unordered_map<ToolProperty::Type, std::shared_ptr<ToolProperty>> m_properties{};
};

#endif  // PROPERTYMANAGER_H

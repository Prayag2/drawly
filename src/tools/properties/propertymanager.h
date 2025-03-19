#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H
#include <QObject>

enum class ToolPropertyType {
    StrokeWidth = 0x00
};
class ToolProperty;

class PropertyManager : public QObject
{
public:
    explicit PropertyManager(QWidget *parent = nullptr);
    std::shared_ptr<ToolProperty> getToolProperty(const ToolPropertyType toolType) const;

private:
    std::unordered_map<ToolPropertyType, std::shared_ptr<ToolProperty>> m_properties {};
};

#endif // PROPERTYMANAGER_H

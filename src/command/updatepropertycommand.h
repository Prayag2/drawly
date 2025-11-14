#ifndef UPDATEPROPERTYCOMMAND_H
#define UPDATEPROPERTYCOMMAND_H

#include "../properties/property.h"
#include "itemcommand.h"
class ApplicationContext;

class UpdatePropertyCommand : public ItemCommand {
public:
    UpdatePropertyCommand(QVector<std::shared_ptr<Item>> items, Property newProperty);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

private:
    Property m_newProperty{};
    std::unordered_map<std::shared_ptr<Item>, Property> m_properties{};
};

#endif  // REMOVEITEMCOMMAND_H

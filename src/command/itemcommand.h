#ifndef ITEMCOMMAND_H
#define ITEMCOMMAND_H

#include <memory>
#include "command.h"
class ApplicationContext;
class Item;

class ItemCommand : public Command {
public:
    ItemCommand(std::shared_ptr<Item> item);
    ~ItemCommand();

protected:
    std::shared_ptr<Item> m_item;
};

#endif // ITEMCOMMAND_H

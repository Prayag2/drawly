#ifndef REMOVEITEMCOMMAND_H
#define REMOVEITEMCOMMAND_H

#include "itemcommand.h"
class ApplicationContext;

class RemoveItemCommand : public ItemCommand {
public:
    RemoveItemCommand(std::shared_ptr<Item> item);

    void execute(ApplicationContext* context) override;
    void undo(ApplicationContext* context) override;
};

#endif // REMOVEITEMCOMMAND_H

#ifndef REMOVEITEMCOMMAND_H
#define REMOVEITEMCOMMAND_H

#include "itemcommand.h"
class ApplicationContext;

class RemoveItemCommand : public ItemCommand {
public:
    RemoveItemCommand(QVector<std::shared_ptr<Item>> items);

    void execute(ApplicationContext* context) override;
    void undo(ApplicationContext* context) override;
};

#endif // REMOVEITEMCOMMAND_H

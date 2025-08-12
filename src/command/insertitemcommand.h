#ifndef INSERTITEMCOMMAND_H
#define INSERTITEMCOMMAND_H

#include "itemcommand.h"
class ApplicationContext;

class InsertItemCommand : public ItemCommand {
public:
    InsertItemCommand(std::shared_ptr<Item> item);
    ~InsertItemCommand();

    void execute(ApplicationContext* context) override;
    void undo(ApplicationContext* context) override;
};

#endif // INSERTITEMCOMMAND_H

#ifndef MOVEITEMCOMMAND_H
#define MOVEITEMCOMMAND_H

#include "itemcommand.h"
class ApplicationContext;

class MoveItemCommand : public ItemCommand {
public:
    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
};

#endif  // MOVEITEMCOMMAND_H

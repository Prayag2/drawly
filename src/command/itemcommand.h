#ifndef ITEMCOMMAND_H
#define ITEMCOMMAND_H

#include <memory>
#include <QVector>
#include "command.h"
class ApplicationContext;
class Item;

class ItemCommand : public Command {
public:
    ItemCommand(QVector<std::shared_ptr<Item>> items);
    ~ItemCommand();

protected:
    QVector<std::shared_ptr<Item>> m_items;
};

#endif // ITEMCOMMAND_H

#include "itemcommand.h"
#include <QDebug>

ItemCommand::ItemCommand(std::shared_ptr<Item> item)
    : m_item{item} {}

ItemCommand::~ItemCommand() {
    qDebug() << "Object deleted: ItemCommand";
}

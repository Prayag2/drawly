#include "itemcommand.h"

#include <QDebug>

ItemCommand::ItemCommand(QVector<std::shared_ptr<Item>> items) : m_items{items} {
}

ItemCommand::~ItemCommand() {
    qDebug() << "Object deleted: ItemCommand";
}

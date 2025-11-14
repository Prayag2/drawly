#ifndef MOVEITEMCOMMAND_H
#define MOVEITEMCOMMAND_H

#include "itemcommand.h"
#include <QPointF>
class ApplicationContext;

class MoveItemCommand : public ItemCommand {
public:
    MoveItemCommand(QVector<std::shared_ptr<Item>> items, QPointF delta);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

private:
    QPointF m_delta;
};

#endif  // MOVEITEMCOMMAND_H

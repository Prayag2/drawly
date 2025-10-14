#include "selectioncontext.h"

#include "../item/item.h"
#include "applicationcontext.h"

SelectionContext::SelectionContext(ApplicationContext *context)
    : QObject{context},
      m_applicationContext{context} {
}

SelectionContext::~SelectionContext() {
    qDebug() << "Object deleted: SelectionContext";
}

std::unordered_set<std::shared_ptr<Item>> &SelectionContext::selectedItems() {
    return m_selectedItems;
}

QRectF SelectionContext::selectionBox() const {
    QRectF selectionBox;
    for (auto item : m_selectedItems) {
        selectionBox |= item->boundingBox();
    }
    return selectionBox;
}

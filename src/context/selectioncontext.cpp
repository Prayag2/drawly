#include "selectioncontext.h"

#include "../command/commandhistory.h"
#include "../command/updatepropertycommand.h"
#include "../data-structures/cachegrid.h"
#include "../item/item.h"
#include "../properties/property.h"
#include "applicationcontext.h"
#include "coordinatetransformer.h"
#include "renderingcontext.h"
#include "spatialcontext.h"

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

// PUBLIC SLOTS
void SelectionContext::updatePropertyOfSelectedItems(Property property) {
    QVector<std::shared_ptr<Item>> items{m_selectedItems.begin(), m_selectedItems.end()};

    auto &commandHistory{m_applicationContext->spatialContext().commandHistory()};
    commandHistory.insert(std::make_shared<UpdatePropertyCommand>(items, property));

    m_applicationContext->renderingContext().markForRender();
    m_applicationContext->renderingContext().markForUpdate();
}

void SelectionContext::reset() {
    selectedItems().clear();
}

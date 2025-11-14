#include "selectiontoolselectstate.h"

#include "../../canvas/canvas.h"
#include "../../components/propertybar.h"
#include "../../context/applicationcontext.h"
#include "../../context/coordinatetransformer.h"
#include "../../context/renderingcontext.h"
#include "../../context/selectioncontext.h"
#include "../../context/spatialcontext.h"
#include "../../context/uicontext.h"
#include "../../data-structures/cachegrid.h"
#include "../../data-structures/quadtree.h"
#include "../../event/event.h"
#include "../../item/item.h"

bool SelectionToolSelectState::mousePressed(ApplicationContext *context) {
    auto &uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        m_lastPos = uiContext.event().pos();

        auto &spatialContext{context->spatialContext()};
        auto &selectionContext{context->selectionContext()};
        auto &renderingContext{context->renderingContext()};
        auto &transformer{spatialContext.coordinateTransformer()};

        QVector<std::shared_ptr<Item>> intersectingItems{
            spatialContext.quadtree().queryItems(transformer.viewToWorld(m_lastPos),
                                                 [](std::shared_ptr<Item> item, auto &pos) {
                                                     return item->boundingBox().contains(pos);
                                                 })};

        bool lockState = true;

        selectionContext.selectedItems().clear();
        if (intersectingItems.empty()) {
            m_isActive = true;
        } else {
            selectionContext.selectedItems().insert(intersectingItems.back());
            m_isActive = false;
            lockState = false;
        }

        context->uiContext().propertyBar().updateToolProperties();
        renderingContext.markForRender();
        renderingContext.markForUpdate();

        return lockState;
    }

    return true;
}

void SelectionToolSelectState::mouseMoved(ApplicationContext *context) {
    auto &renderingContext{context->renderingContext()};
    auto &spatialContext{context->spatialContext()};
    renderingContext.canvas().setCursor(Qt::ArrowCursor);

    auto &painter{renderingContext.overlayPainter()};
    if (!m_isActive) {
        return;
    }

    auto &uiContext{context->uiContext()};
    auto &transformer{spatialContext.coordinateTransformer()};
    auto &selectionContext{context->selectionContext()};
    auto &selectedItems{selectionContext.selectedItems()};

    renderingContext.canvas().overlay()->fill(Qt::transparent);

    QPointF curPos{uiContext.event().pos()};

    QRectF selectionBox{m_lastPos, curPos};
    QRectF worldSelectionBox{transformer.viewToWorld(selectionBox)};

    QVector<std::shared_ptr<Item>> intersectingItems{
        spatialContext.quadtree().queryItems(worldSelectionBox,
                                             [](std::shared_ptr<Item> item, const QRectF &rect) {
                                                 return rect.contains(item->boundingBox());
                                             })};

    selectedItems = std::unordered_set(intersectingItems.begin(), intersectingItems.end());
    context->uiContext().propertyBar().updateToolProperties();

    QPainter &overlayPainter{renderingContext.overlayPainter()};
    overlayPainter.save();

    // TODO: Remove magic numbers
    QPen pen{QColor{67, 135, 244, 200}};
    overlayPainter.setPen(pen);

    overlayPainter.drawRect(selectionBox);
    overlayPainter.fillRect(selectionBox, QColor{67, 135, 244, 50});

    overlayPainter.restore();

    renderingContext.markForRender();
    renderingContext.markForUpdate();
}

bool SelectionToolSelectState::mouseReleased(ApplicationContext *context) {
    if (m_isActive) {
        auto &renderingContext{context->renderingContext()};

        renderingContext.canvas().overlay()->fill(Qt::transparent);
        renderingContext.markForUpdate();

        m_isActive = false;
    }

    return false;
}

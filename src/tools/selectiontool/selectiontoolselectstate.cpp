#include "selectiontoolselectstate.h"
#include "../../context/applicationcontext.h"
#include "../../canvas/canvas.h"
#include "../../item/item.h"
#include "../../context/applicationcontext.h"
#include "../../context/coordinatetransformer.h"
#include "../../context/selectioncontext.h"
#include "../../context/renderingcontext.h"
#include "../../context/spatialcontext.h"
#include "../../context/uicontext.h"
#include "../../data-structures/cachegrid.h"
#include "../../data-structures/quadtree.h"
#include "../../event/event.h"

// TODO: Add the ability to select a shape by just clicking it
void SelectionToolSelectState::mousePressed(ApplicationContext* context) {
    auto& uiContext{context->uiContext()};
    if (uiContext.event().button() == Qt::LeftButton) {
        m_lastPos = uiContext.event().pos();
        m_isActive = true;
    }
}

void SelectionToolSelectState::mouseMoved(ApplicationContext* context) {
    auto& renderingContext{context->renderingContext()};
    renderingContext.canvas().setCursor(Qt::ArrowCursor);

    if (!m_isActive)
        return;

    auto& uiContext{context->uiContext()};
    auto& spatialContext{context->spatialContext()};
    auto& transformer{spatialContext.coordinateTransformer()};
    auto& selectionContext{context->selectionContext()};
    auto& selectedItems{selectionContext.selectedItems()};

    renderingContext.canvas().overlay()->fill(Qt::transparent);

    QPointF curPos{uiContext.event().pos()};

    QRectF selectionBox{m_lastPos, curPos};
    QRectF worldSelectionBox{transformer.viewToWorld(selectionBox)};

    QVector<std::shared_ptr<Item>> intersectingItems{spatialContext.quadtree().queryItems(
        worldSelectionBox, [](std::shared_ptr<Item> item, const QRectF& rect) {
            return rect.contains(item->boundingBox());
        })};

    selectedItems = std::unordered_set(intersectingItems.begin(), intersectingItems.end());

    QPainter& overlayPainter{renderingContext.overlayPainter()};
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

void SelectionToolSelectState::mouseReleased(ApplicationContext* context) {
    if (m_isActive) {
        auto& renderingContext{context->renderingContext()};

        renderingContext.canvas().overlay()->fill(Qt::transparent);
        renderingContext.markForUpdate();

        m_isActive = false;
    }
}

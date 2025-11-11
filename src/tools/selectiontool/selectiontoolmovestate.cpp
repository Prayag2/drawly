#include "selectiontoolmovestate.h"

#include "../../canvas/canvas.h"
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

bool SelectionToolMoveState::mousePressed(ApplicationContext *context) {
    auto &uiContext{context->uiContext()};
    auto &renderingContext{context->renderingContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        renderingContext.canvas().setCursor(Qt::ClosedHandCursor);

        m_lastPos = uiContext.event().pos();
        m_isActive = true;
    }

    return true;
}

void SelectionToolMoveState::mouseMoved(ApplicationContext *context) {
    auto &renderingContext{context->renderingContext()};

    if (!m_isActive) {
        renderingContext.canvas().setCursor(Qt::OpenHandCursor);
        return;
    }

    auto &spatialContext{context->spatialContext()};
    auto &selectionContext{context->selectionContext()};
    auto &transformer{spatialContext.coordinateTransformer()};

    auto &selectedItems{selectionContext.selectedItems()};

    QPointF curPos{context->uiContext().event().pos()};

    QPointF worldCurPos{transformer.viewToWorld(curPos)};
    QPointF worldLastPos{transformer.viewToWorld(m_lastPos)};
    QPointF displacement{worldCurPos - worldLastPos};

    for (auto item : selectedItems) {
        QRectF oldBoundingBox{item->boundingBox()};

        spatialContext.cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
        item->translate(displacement);
        spatialContext.cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());

        spatialContext.quadtree().updateItem(item, oldBoundingBox);
    }

    m_lastPos = curPos;
    renderingContext.markForRender();
    renderingContext.markForUpdate();
}

bool SelectionToolMoveState::mouseReleased(ApplicationContext *context) {
    auto &renderingContext{context->renderingContext()};
    renderingContext.canvas().setCursor(Qt::OpenHandCursor);
    m_isActive = false;

    return false;
}

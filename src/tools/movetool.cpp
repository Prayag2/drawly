#include "movetool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/spatialcontext.h"
#include "../context/uicontext.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/item.h"

MoveTool::MoveTool() {
    m_cursor = QCursor(Qt::OpenHandCursor);
}

void MoveTool::mousePressed(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};
    auto button{uiContext.event().button()};

    if (button == Qt::LeftButton || button == Qt::MiddleButton) {
        SpatialContext &spatialContext{context->spatialContext()};
        RenderingContext &renderingContext{context->renderingContext()};

        m_isActive = true;

        m_initialOffsetPos = spatialContext.offsetPos();
        m_initialPos = uiContext.event().pos();

        renderingContext.canvas().setCursor(Qt::ClosedHandCursor);
    }
};

void MoveTool::mouseMoved(ApplicationContext *context) {
    if (m_isActive) {
        SpatialContext &spatialContext{context->spatialContext()};
        RenderingContext &renderingContext{context->renderingContext()};
        UIContext &uiContext{context->uiContext()};

        qreal zoom{renderingContext.zoomFactor()};
        QPointF newPoint{m_initialOffsetPos * zoom - uiContext.event().pos() + m_initialPos};

        spatialContext.setOffsetPos(newPoint / zoom);

        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

void MoveTool::mouseReleased(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};
    auto button{uiContext.event().button()};

    if (button == Qt::LeftButton || button == Qt::MiddleButton) {
        RenderingContext &renderingContext{context->renderingContext()};

        renderingContext.canvas().setCursor(Qt::OpenHandCursor);
        m_isActive = false;
    }
};

Tool::Type MoveTool::type() const {
    return Tool::Move;
}

QString MoveTool::tooltip() const {
    return "Move Tool";
}

IconManager::Icon MoveTool::icon() const {
    return IconManager::TOOL_MOVE;
}

#include "movetool.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/spatialcontext.h"
#include "../context/uicontext.h"
#include "../event/event.h"
#include "../item/item.h"

MoveTool::MoveTool() {
    m_cursor = QCursor(Qt::OpenHandCursor);
}

QString MoveTool::iconAlt() const {
    return "";
}

void MoveTool::mousePressed(ApplicationContext* context) {
    UIContext& uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext& spatialContext{context->spatialContext()};

        m_isActive = true;

        m_initialOffsetPos = spatialContext.offsetPos();
        m_initialPos = uiContext.event().pos();
    }
};

void MoveTool::mouseMoved(ApplicationContext* context) {
    if (m_isActive) {
        SpatialContext& spatialContext{context->spatialContext()};
        RenderingContext& renderingContext{context->renderingContext()};
        UIContext& uiContext{context->uiContext()};

        qreal zoom{renderingContext.zoomFactor()};
        QPointF newPoint{m_initialOffsetPos * zoom - uiContext.event().pos() +
                         m_initialPos};

        spatialContext.setOffsetPos(newPoint / zoom);

        renderingContext.canvas().setCursor(Qt::ClosedHandCursor);

        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

void MoveTool::mouseReleased(ApplicationContext* context) {
    UIContext& uiContext{context->uiContext()};
    if (uiContext.event().button() == Qt::LeftButton) {
        RenderingContext& renderingContext{context->renderingContext()};

        renderingContext.canvas().setCursor(Qt::OpenHandCursor);
        m_isActive = false;
    }
};

ToolID MoveTool::id() const {
    return ToolID::MoveTool;
}

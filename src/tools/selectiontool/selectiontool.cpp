#include "selectiontool.h"
#include "selectiontoolstate.h"
#include "selectiontoolmovestate.h"
#include "selectiontoolselectstate.h"
#include "../../context/applicationcontext.h"
#include "../../context/selectioncontext.h"
#include "../../context/spatialcontext.h"
#include "../../context/uicontext.h"
#include "../../context/coordinatetransformer.h"
#include "../../context/uicontext.h"
#include "../../event/event.h"

SelectionTool::SelectionTool() {
    m_cursor = QCursor(Qt::ArrowCursor);

    m_moveState = std::make_shared<SelectionToolMoveState>();
    m_selectState = std::make_shared<SelectionToolSelectState>();
}

QString SelectionTool::iconAlt() const {
    return "󰆿";
}

void SelectionTool::mousePressed(ApplicationContext* context) {
    getCurrentState(context)->mousePressed(context);
};

void SelectionTool::mouseMoved(ApplicationContext* context) {
    getCurrentState(context)->mouseMoved(context);
};

void SelectionTool::mouseReleased(ApplicationContext* context) {
    getCurrentState(context)->mouseReleased(context);
};

std::shared_ptr<SelectionToolState> SelectionTool::getCurrentState(ApplicationContext* context) {
    auto& selectionContext{context->selectionContext()};
    auto& uiContext{context->uiContext()};
    auto& transformer{context->spatialContext().coordinateTransformer()};

    QPointF worldCurPos{transformer.viewToWorld(uiContext.event().pos())};

    // TODO: Implement resizing and rotation as well
    if (selectionContext.selectionBox().contains(worldCurPos)) {
        return m_moveState;
    } else {
        return m_selectState;
    }
}

ToolID SelectionTool::id() const {
    return ToolID::SelectionTool;
};

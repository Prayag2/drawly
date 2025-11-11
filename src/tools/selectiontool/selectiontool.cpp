#include "selectiontool.h"

#include "../../context/applicationcontext.h"
#include "../../context/coordinatetransformer.h"
#include "../../context/selectioncontext.h"
#include "../../context/spatialcontext.h"
#include "../../context/uicontext.h"
#include "../../event/event.h"
#include "../../item/item.h"
#include "selectiontoolmovestate.h"
#include "selectiontoolselectstate.h"
#include "selectiontoolstate.h"
#include <set>

SelectionTool::SelectionTool() {
    m_cursor = QCursor(Qt::ArrowCursor);

    m_moveState = std::make_shared<SelectionToolMoveState>();
    m_selectState = std::make_shared<SelectionToolSelectState>();
}

QString SelectionTool::iconAlt() const {
    return "󰆿";
}

void SelectionTool::mousePressed(ApplicationContext *context) {
    m_stateLocked = getCurrentState(context)->mousePressed(context);
};

void SelectionTool::mouseMoved(ApplicationContext *context) {
    getCurrentState(context)->mouseMoved(context);
};

void SelectionTool::mouseReleased(ApplicationContext *context) {
    m_stateLocked = getCurrentState(context)->mouseReleased(context);
};

std::shared_ptr<SelectionToolState> SelectionTool::getCurrentState(ApplicationContext *context) {
    if (m_stateLocked)
        return m_curState;

    auto &selectionContext{context->selectionContext()};
    auto &uiContext{context->uiContext()};
    auto &transformer{context->spatialContext().coordinateTransformer()};

    QPointF worldCurPos{transformer.viewToWorld(uiContext.event().pos())};

    // TODO: Implement resizing and rotation as well
    if (selectionContext.selectionBox().contains(worldCurPos)) {
        return m_curState = m_moveState;
    } else {
        return m_curState = m_selectState;
    }
}

const QVector<Property::Type> SelectionTool::properties() const {
    ApplicationContext *context{ApplicationContext::instance()};
    auto& selectedItems{context->selectionContext().selectedItems()};

    std::set<Property::Type> result{};
    for (const auto& item : selectedItems) {
        for (const auto& property : item->properties()) {
            result.insert(property);
        }
    }

    return QVector<Property::Type>(result.begin(), result.end());
}

ToolID SelectionTool::id() const {
    return ToolID::SelectionTool;
};

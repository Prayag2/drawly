/*
* Drawy - A simple brainstorming tool with an infinite canvas
* Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "selectiontool.h"

#include "../../command/commandhistory.h"
#include "../../command/moveitemcommand.h"
#include "../../common/constants.h"
#include "../../context/applicationcontext.h"
#include "../../context/coordinatetransformer.h"
#include "../../context/renderingcontext.h"
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

void SelectionTool::keyPressed(ApplicationContext *context) {
    auto &selectedItems{context->selectionContext().selectedItems()};
    if (selectedItems.empty())
        return;

    auto &event{context->uiContext().event()};
    auto &commandHistory{context->spatialContext().commandHistory()};
    QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};

    int delta{Common::translationDelta};
    if (event.modifiers() & Qt::ShiftModifier)
        delta = Common::shiftTranslationDelta;

    bool updated{true};
    switch (event.key()) {
        case Qt::Key_Left:
            commandHistory.insert(std::make_shared<MoveItemCommand>(items, QPoint{-delta, 0}));
            break;
        case Qt::Key_Right:
            commandHistory.insert(std::make_shared<MoveItemCommand>(items, QPoint{delta, 0}));
            break;
        case Qt::Key_Up:
            commandHistory.insert(std::make_shared<MoveItemCommand>(items, QPoint{0, -delta}));
            break;
        case Qt::Key_Down:
            commandHistory.insert(std::make_shared<MoveItemCommand>(items, QPoint{0, delta}));
            break;
        default:
            updated = false;
    }

    if (updated) {
        context->renderingContext().markForRender();
        context->renderingContext().markForUpdate();
    }
}

const QVector<Property::Type> SelectionTool::properties() const {
    ApplicationContext *context{ApplicationContext::instance()};
    auto &selectedItems{context->selectionContext().selectedItems()};

    std::set<Property::Type> result{};
    for (const auto &item : selectedItems) {
        for (const auto &property : item->propertyTypes()) {
            result.insert(property);
        }
    }

    return QVector<Property::Type>(result.begin(), result.end());
}

Tool::Type SelectionTool::type() const {
    return Tool::Selection;
};

QString SelectionTool::tooltip() const {
    return "Selection Tool";
}

IconManager::Icon SelectionTool::icon() const {
    return IconManager::TOOL_SELECTION;
}

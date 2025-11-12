#include "actionmanager.h"

#include "../command/commandhistory.h"
#include "../components/toolbar.h"
#include "../context/applicationcontext.h"
#include "../context/renderingcontext.h"
#include "../context/spatialcontext.h"
#include "../context/selectioncontext.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"
#include "../context/coordinatetransformer.h"
#include "../components/propertybar.h"
#include "../context/uicontext.h"
#include "action.h"
#include "keybindmanager.h"

ActionManager::ActionManager(ApplicationContext *context) : m_context{context}, QObject(context) {
    KeybindManager &keybindManager{m_context->uiContext().keybindManager()};

    Action *undoAction{new Action{"Undo", "Undo last action", [&]() { this->undo(); }, context}};

    Action *redoAction{
        new Action{"Redo", "Redo last undone action", [&]() { this->redo(); }, context}};

    Action *zoomInAction{new Action{"Zoom In", "Zoom in", [&]() { this->zoomIn(); }, context}};

    Action *zoomOutAction{new Action{"Zoom Out", "Zoom out", [&]() { this->zoomOut(); }, context}};

    Action *increaseThicknessAction{new Action{"Increase Thickness",
                                               "Increase brush thickness",
                                               [&]() { this->increaseThickness(); },
                                               context}};

    Action *decreaseThicknessAction{new Action{"Decrease Thickness",
                                               "Decrease brush thickness",
                                               [&]() { this->decreaseThickness(); },
                                               context}};

    Action *freeformToolAction{new Action{"Freeform Tool",
                                          "Switch to freeform drawing tool",
                                          [&]() { this->switchToFreeformTool(); },
                                          context}};

    Action *eraserToolAction{new Action{"Eraser Tool",
                                        "Switch to eraser tool",
                                        [&]() { this->switchToEraserTool(); },
                                        context}};

    Action *selectionToolAction{new Action{"Selection Tool",
                                           "Switch to selection tool",
                                           [&]() { this->switchToSelectionTool(); },
                                           context}};

    Action *rectangleToolAction{new Action{"Rectangle Tool",
                                           "Switch to rectangle drawing tool",
                                           [&]() { this->switchToRectangleTool(); },
                                           context}};

    Action *ellipseToolAction{new Action{"Ellipse Tool",
                                         "Switch to ellipse drawing tool",
                                         [&]() { this->switchToEllipseTool(); },
                                         context}};

    Action *lineToolAction{new Action{"Line Tool",
                                      "Switch to line drawing tool",
                                      [&]() { this->switchToLineTool(); },
                                      context}};

    Action *arrowToolAction{new Action{"Arrow Tool",
                                       "Switch to arrow drawing tool",
                                       [&]() { this->switchToArrowTool(); },
                                       context}};

    Action *moveToolAction{new Action{"Move Tool",
                                      "Switch to move tool",
                                      [&]() { this->switchToMoveTool(); },
                                      context}};

    Action *selectAllAction{new Action{"Select All",
                                      "Select all items",
                                      [&, context]() {
                                          this->switchToSelectionTool();

                                          auto allItems{context->spatialContext().quadtree().getAllItems()};
                                          context->selectionContext().selectedItems().insert(allItems.begin(), allItems.end());

                                          context->uiContext().propertyBar().updateToolProperties();
                                          context->renderingContext().markForRender();
                                          context->renderingContext().markForUpdate();
                                      },
                                      context}};

    Action *deleteAction{new Action{"Delete",
                                      "Deletes selected items",
                                      [&, context]() { 
                                          auto& itemsToBeDeleted{context->selectionContext().selectedItems()};
                                          auto& transformer{context->spatialContext().coordinateTransformer()};
                                          for (auto& item : itemsToBeDeleted) {
                                              context->spatialContext().cacheGrid().markDirty(transformer.worldToGrid(item->boundingBox()).toRect());
                                              context->spatialContext().quadtree().deleteItem(item);
                                          }

                                          context->renderingContext().markForRender();
                                          context->renderingContext().markForUpdate();

                                          context->selectionContext().selectedItems().clear();
                                      },
                                      context}};

    keybindManager.addKeybinding(undoAction, "Ctrl+Z");
    keybindManager.addKeybinding(redoAction, "Ctrl+Y");
    keybindManager.addKeybinding(redoAction, "Ctrl+Shift+Z");
    keybindManager.addKeybinding(zoomInAction, "Ctrl++");
    keybindManager.addKeybinding(zoomOutAction, "Ctrl+-");
    keybindManager.addKeybinding(increaseThicknessAction, "]");
    keybindManager.addKeybinding(decreaseThicknessAction, "[");
    keybindManager.addKeybinding(freeformToolAction, "P");
    keybindManager.addKeybinding(freeformToolAction, "B");
    keybindManager.addKeybinding(eraserToolAction, "E");
    keybindManager.addKeybinding(selectionToolAction, "S");
    keybindManager.addKeybinding(rectangleToolAction, "R");
    keybindManager.addKeybinding(ellipseToolAction, "O");
    keybindManager.addKeybinding(lineToolAction, "L");
    keybindManager.addKeybinding(arrowToolAction, "A");
    keybindManager.addKeybinding(moveToolAction, "M");
    keybindManager.addKeybinding(selectAllAction, "Ctrl+A");
    keybindManager.addKeybinding(deleteAction, "Delete");
}

void ActionManager::undo() {
    m_context->spatialContext().commandHistory().undo();
    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}

void ActionManager::redo() {
    m_context->spatialContext().commandHistory().redo();
    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}

void ActionManager::zoomIn() {
    m_context->renderingContext().setZoomFactor(1);
}

void ActionManager::zoomOut() {
    m_context->renderingContext().setZoomFactor(-1);
}

void ActionManager::switchToFreeformTool() {
    m_context->uiContext().toolBar().changeTool(ToolID::FreeformTool);
}

void ActionManager::switchToEraserTool() {
    m_context->uiContext().toolBar().changeTool(ToolID::EraserTool);
}

void ActionManager::switchToRectangleTool() {
    m_context->uiContext().toolBar().changeTool(ToolID::RectangleTool);
}

void ActionManager::switchToEllipseTool() {
    m_context->uiContext().toolBar().changeTool(ToolID::EllipseTool);
}

void ActionManager::switchToLineTool() {
    m_context->uiContext().toolBar().changeTool(ToolID::LineTool);
}

void ActionManager::switchToArrowTool() {
    m_context->uiContext().toolBar().changeTool(ToolID::ArrowTool);
}

void ActionManager::switchToMoveTool() {
    m_context->uiContext().toolBar().changeTool(ToolID::MoveTool);
}

void ActionManager::switchToSelectionTool() {
    m_context->uiContext().toolBar().changeTool(ToolID::SelectionTool);
}

void ActionManager::increaseThickness() {
    // TODO: implement
}

void ActionManager::decreaseThickness() {
    // TODO: implement
}

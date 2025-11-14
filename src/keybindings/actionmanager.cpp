#include "actionmanager.h"

#include "../command/commandhistory.h"
#include "../command/removeitemcommand.h"
#include "../components/propertybar.h"
#include "../components/toolbar.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/renderingcontext.h"
#include "../context/selectioncontext.h"
#include "../context/spatialcontext.h"
#include "../context/uicontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../serializer/loader.h"
#include "../serializer/serializer.h"
#include "action.h"
#include "keybindmanager.h"
#include <memory>

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

    Action *selectAllAction{new Action{
        "Select All",
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

    Action *deleteAction{new Action{
        "Delete",
        "Deletes selected items",
        [&, context]() {
            auto &selectedItems{context->selectionContext().selectedItems()};
            auto &transformer{context->spatialContext().coordinateTransformer()};
            auto &commandHistory{context->spatialContext().commandHistory()};

            QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
            commandHistory.insert(std::make_shared<RemoveItemCommand>(items));

            context->renderingContext().markForRender();
            context->renderingContext().markForUpdate();

            context->selectionContext().selectedItems().clear();
        },
        context}};

    Action *saveAction{new Action{"Save",
                                  "Save canvas",
                                  [&, context]() {
                                      Serializer serializer{};

                                      serializer.serialize(context);
                                      serializer.saveToFile();
                                  },
                                  context}};

    Action *openFileAction{new Action{"Open File",
                                      "Open an existing file",
                                      [&, context]() {
                                          Loader loader{};
                                          loader.loadFromFile(context);
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
    keybindManager.addKeybinding(saveAction, "Ctrl+S");
    keybindManager.addKeybinding(openFileAction, "Ctrl+O");
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
    m_context->renderingContext().updateZoomFactor(1);
}

void ActionManager::zoomOut() {
    m_context->renderingContext().updateZoomFactor(-1);
}

void ActionManager::switchToFreeformTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Freeform);
}

void ActionManager::switchToEraserTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Eraser);
}

void ActionManager::switchToRectangleTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Rectangle);
}

void ActionManager::switchToEllipseTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Ellipse);
}

void ActionManager::switchToLineTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Line);
}

void ActionManager::switchToArrowTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Arrow);
}

void ActionManager::switchToMoveTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Move);
}

void ActionManager::switchToSelectionTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Selection);
}

void ActionManager::increaseThickness() {
    // TODO: implement
}

void ActionManager::decreaseThickness() {
    // TODO: implement
}

#include "texttool.h"

#include "../common/constants.h"
#include "../canvas/canvas.h"
#include "../command/commandhistory.h"
#include "../command/insertitemcommand.h"
#include "../context/applicationcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/renderingcontext.h"
#include "../context/spatialcontext.h"
#include "../context/uicontext.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/factory/textfactory.h"
#include "../keybindings/keybindmanager.h"
#include <QClipboard>
#include <QGuiApplication>

/*
 * FIXME: This file needs some SERIOUS refactoring which, unfortunately, time is not allowing me to do.
 *        Feel free to open pull requests though. I have made sure to implement all necessary functionalities.
 *        AI slop will be rejected :D 
 *        Put your heart and soul into the work you do and see yourself succeed.
 */

TextTool::TextTool(const PropertyManager &propertyManager) {
    m_cursor = QCursor(Qt::CrossCursor);
    m_itemFactory = std::make_unique<TextFactory>();
}

void TextTool::mousePressed(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};
        QuadTree &quadTree{spatialContext.quadtree()};
        CommandHistory &commandHistory{spatialContext.commandHistory()};

        QPointF worldPos{transformer.viewToWorld(uiContext.event().pos())};
        QVector<std::shared_ptr<Item>> intersectingItems{
            quadTree.queryItems(worldPos, [](std::shared_ptr<Item> item, const QPointF &point) {
                return item->type() == Item::Text && item->boundingBox().contains(point);
            })};

        if (intersectingItems.empty()) {
            if (m_curItem == nullptr) {
                m_curItem = std::dynamic_pointer_cast<Text>(m_itemFactory->create());
                m_curItem->setBoundingBoxPadding(10 * renderingContext.canvas().scale());
                m_curItem->createTextBox(transformer.viewToWorld(uiContext.event().pos()));
                commandHistory.insert(
                    std::make_shared<InsertItemCommand>(QVector<std::shared_ptr<Item>>{m_curItem}));
            } else {
                m_curItem->setMode(Text::NORMAL);
                spatialContext.cacheGrid().markDirty(
                    transformer.worldToGrid(m_curItem->boundingBox()).toRect());

                // enable keybindings again
                uiContext.keybindManager().enable();
                
                if (m_curItem->text().isEmpty()) {
                    quadTree.deleteItem(m_curItem);
                }

                m_curItem = nullptr;
                renderingContext.markForRender();
                renderingContext.markForUpdate();
                return;
            }
        } else {
            if (m_curItem != nullptr) {
                m_curItem->setMode(Text::NORMAL);
                spatialContext.cacheGrid().markDirty(
                    transformer.worldToGrid(m_curItem->boundingBox()).toRect());
            }

            m_curItem = std::dynamic_pointer_cast<Text>(intersectingItems.back());
            m_curItem->setCaret(worldPos);

            spatialContext.cacheGrid().markDirty(
                transformer.worldToGrid(m_curItem->boundingBox()).toRect());

            m_isSelecting = true;
            m_mouseMoved = false;

            int lineNumber{m_curItem->getLineFromY(worldPos.y())};
            qsizetype index{m_curItem->getIndexFromX(worldPos.x(), lineNumber)};
            m_curItem->setSelectionStart(index);
            m_curItem->setSelectionEnd(Text::INVALID);
        }

        m_curItem->setMode(Text::EDIT);
        uiContext.keybindManager().disable();

        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }

    m_isDrawing = true;
};

void TextTool::mouseMoved(ApplicationContext *context) {
    SpatialContext &spatialContext{context->spatialContext()};
    CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
    RenderingContext &renderingContext{context->renderingContext()};
    UIContext &uiContext{context->uiContext()};
    QuadTree &quadTree{spatialContext.quadtree()};
    m_mouseMoved = true;

    QPointF worldPos{transformer.viewToWorld(uiContext.event().pos())};
    QVector<std::shared_ptr<Item>> intersectingItems{
        quadTree.queryItems(worldPos, [](std::shared_ptr<Item> item, const QPointF &point) {
            return item->type() == Item::Text && item->boundingBox().contains(point);
        })};

    if (!intersectingItems.empty()) {
        renderingContext.canvas().setCursor(Qt::IBeamCursor);
    } else {
        renderingContext.canvas().setCursor(Qt::CrossCursor);
    }

    if (m_isSelecting) {
        int lineNumber{m_curItem->getLineFromY(worldPos.y())};
        qsizetype curIndex{m_curItem->getIndexFromX(worldPos.x(), lineNumber)};
        bool isLeft{curIndex < m_curItem->selectionStart()};

        qsizetype curStart {m_curItem->selectionStart()};
        qsizetype curEnd {m_curItem->selectionEnd()};

        if (m_doubleClicked) {
            if (isLeft) {
                m_curItem->setSelectionStart(std::max(curStart, curEnd));
                m_curItem->setSelectionEnd(m_curItem->getPrevBreak(curIndex));
            } else {
                m_curItem->setSelectionStart(std::min(curStart, curEnd));
                m_curItem->setSelectionEnd(m_curItem->getNextBreak(curIndex));
            }
        } else if (m_tripleClicked) {
            auto [start, end] = m_curItem->getLineRange(curIndex);
            if (isLeft) {
                m_curItem->setSelectionStart(std::max(curStart, curEnd));
                m_curItem->setSelectionEnd(start);
            } else {
                m_curItem->setSelectionStart(std::min(curStart, curEnd));
                m_curItem->setSelectionEnd(end + 1);
            }
        } else {
            m_curItem->setSelectionEnd(curIndex);
        }

        spatialContext.cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());
        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

void TextTool::mouseReleased(ApplicationContext *context) {
    m_isSelecting = false;
    m_doubleClicked = false;
    m_tripleClicked = false;
};

void TextTool::mouseDoubleClick(ApplicationContext *context) {
    m_doubleClicked = true;
    if (!m_mouseMoved) {
        m_isSelecting = true;

        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};
        UIContext &uiContext{context->uiContext()};

        QPointF worldPos{transformer.viewToWorld(uiContext.event().pos())};

        int lineNumber{m_curItem->getLineFromY(worldPos.y())};
        qsizetype curIndex{m_curItem->getIndexFromX(worldPos.x(), lineNumber)};

        m_curItem->setSelectionStart(m_curItem->getPrevBreak(curIndex));
        m_curItem->setSelectionEnd(m_curItem->getNextBreak(curIndex));

        spatialContext.cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());
        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

void TextTool::mouseTripleClick(ApplicationContext *context) {
    m_tripleClicked = true;
    if (!m_mouseMoved) {
        m_isSelecting = true;

        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};
        UIContext &uiContext{context->uiContext()};

        QPointF worldPos{transformer.viewToWorld(uiContext.event().pos())};

        int lineNumber{m_curItem->getLineFromY(worldPos.y())};
        qsizetype curIndex{m_curItem->getIndexFromX(worldPos.x(), lineNumber)};

        auto [start, end] = m_curItem->getLineRange(curIndex);
        m_curItem->setSelectionStart(start);
        m_curItem->setSelectionEnd(end + 1);

        spatialContext.cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());
        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

// TODO: Refactor, refactor, refactor!
void TextTool::keyPressed(ApplicationContext *context) {
    Event &ev{context->uiContext().event()};

    if (ev.key() == Qt::Key_Escape) {
        m_curItem->setMode(Text::NORMAL);
        context->uiContext().keybindManager().enable();
        m_curItem = nullptr;

        context->spatialContext().cacheGrid().markAllDirty();
        context->renderingContext().markForRender();
        context->renderingContext().markForUpdate();
    }

    if (m_curItem != nullptr && m_curItem->mode() == Text::EDIT) {
        qsizetype caret{m_curItem->caret()};
        const QString& text{m_curItem->text()};
        qsizetype size{text.size()};

        auto handleDefaultCase = [&](){
            if (ev.text().isEmpty())
                return;

            m_curItem->deleteSelection();
            m_curItem->insertText(ev.text());
        };

        // FIXME: What the fuck
        switch (ev.key()) {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                m_curItem->insertText("\n");
                break;
            case Qt::Key_Left: {
                qsizetype newIndex{std::max(static_cast<qsizetype>(0), caret - 1)};
                if (ev.modifiers() & Qt::ControlModifier) {
                    qsizetype curPos{m_curItem->caret()};

                    if (ev.modifiers() & Qt::ShiftModifier) {
                        qsizetype pos{m_curItem->selectionEnd()};
                        m_curItem->setSelectionEnd(m_curItem->getPrevBreak(pos == Text::INVALID ? curPos : pos));
                    } else {
                        m_curItem->setCaret(m_curItem->getPrevBreak(curPos));
                    }
                } else if (ev.modifiers() & Qt::ShiftModifier) {
                    qsizetype selEnd{m_curItem->selectionEnd()};
                    m_curItem->setSelectionEnd(selEnd == -1 ? newIndex : selEnd - 1);
                } else {
                    m_curItem->setCaret(newIndex);
                }
                break;
            } case Qt::Key_Right: {
                qsizetype newIndex{std::min(size, caret + 1)};
                if (ev.modifiers() & Qt::ControlModifier) {
                    qsizetype curPos{m_curItem->caret()};

                    if (ev.modifiers() & Qt::ShiftModifier) {
                        qsizetype pos{m_curItem->selectionEnd()};
                        m_curItem->setSelectionEnd(m_curItem->getNextBreak( pos == Text::INVALID ? curPos : pos));
                    } else {
                        m_curItem->setCaret(m_curItem->getNextBreak(curPos));
                    }
                } else if (ev.modifiers() & Qt::ShiftModifier) {
                    qsizetype selEnd{m_curItem->selectionEnd()};
                    m_curItem->setSelectionEnd(selEnd == -1 ? newIndex : selEnd + 1);
                } else {
                    m_curItem->setCaret(newIndex);
                }
                break;
            }
            case Qt::Key_Backspace: {
                if (m_curItem->hasSelection()) {
                    m_curItem->deleteSelection();
                    break;
                }

                if ((ev.modifiers() & Qt::ControlModifier) && (ev.modifiers() & Qt::ShiftModifier)) {
                    qsizetype prevLine{-1};
                    for (qsizetype pos{caret - 1}; pos >= 0; pos--) {
                        if (text[pos] == '\n') {
                            prevLine = pos;
                            break;
                        }
                    }

                    if (prevLine == caret - 1)
                        prevLine--;

                    m_curItem->deleteSubStr(prevLine + 1, caret - 1);
                    m_curItem->setCaret(prevLine + 1);
                    break;
                }

                if (ev.modifiers() & Qt::ControlModifier) {
                    qsizetype prevBreak{m_curItem->getPrevBreak(caret)};
                    m_curItem->deleteSubStr(prevBreak, caret - 1);
                    m_curItem->setCaret(prevBreak);
                    break;
                }

                caret--;
                m_curItem->deleteSubStr(caret, caret);
                m_curItem->setCaret(caret);
                break;
            }
            case Qt::Key_Delete: {
                if (m_curItem->hasSelection()) {
                    m_curItem->deleteSelection();
                    break;
                }

                if ((ev.modifiers() & Qt::ControlModifier) && (ev.modifiers() & Qt::ShiftModifier)) {
                    qsizetype nextLine{size};
                    for (qsizetype pos{caret}; pos < size; pos++) {
                        if (text[pos] == '\n') {
                            nextLine = pos;
                            break;
                        }
                    }

                    m_curItem->deleteSubStr(caret, nextLine - 1);
                    break;
                }

                if (ev.modifiers() & Qt::ControlModifier) {
                    qsizetype nextBreak{m_curItem->getNextBreak(caret)};
                    m_curItem->deleteSubStr(caret, nextBreak - 1);
                    break;
                }

                m_curItem->deleteSubStr(caret, caret);
                break;
            }
            case Qt::Key_Up: {
                qsizetype prevLineEnd{text.lastIndexOf("\n", caret - 1)};

                if (prevLineEnd == -1)
                    break;

                qsizetype prevLineStart{text.lastIndexOf("\n", prevLineEnd - 1) + 1};
                qsizetype pos{m_curItem->caretPosInLine()};

                qsizetype length{prevLineEnd - prevLineStart + 1};
                if (length >= pos) {
                    m_curItem->setCaret(prevLineStart + pos - 1, false);
                } else {
                    m_curItem->setCaret(prevLineEnd, false);
                }
                break;
            }
            case Qt::Key_Down: {
                qsizetype nextLineStart{text.indexOf("\n", caret)};

                if (nextLineStart == -1)
                    break;

                qsizetype nextLineEnd{text.indexOf("\n", ++nextLineStart)};
                if (nextLineEnd == -1)
                    nextLineEnd = text.size();

                qsizetype pos{m_curItem->caretPosInLine()};

                qsizetype length{nextLineEnd - nextLineStart + 1};
                if (length >= pos) {
                    m_curItem->setCaret(nextLineStart + pos - 1, false);
                } else {
                    m_curItem->setCaret(nextLineEnd, false);
                }
                break;
            } case Qt::Key_A: {
                if (ev.modifiers() & Qt::ControlModifier) {
                    m_curItem->setSelectionStart(0);
                    m_curItem->setSelectionEnd(text.size());
                } else {
                    handleDefaultCase();
                }
                break;
            } case Qt::Key_C: {
                if (ev.modifiers() & Qt::ControlModifier) {
                    if (m_curItem->hasSelection()) {
                        QGuiApplication::clipboard()->setText(m_curItem->selectedText());
                    }
                } else {
                    handleDefaultCase();
                }
                break;
            } case Qt::Key_V: {
                if (ev.modifiers() & Qt::ControlModifier) {
                    ev.setText(QGuiApplication::clipboard()->text()); 
                }

                handleDefaultCase();
                break;
            } default: {
                handleDefaultCase();
            }
        }

        context->spatialContext().quadtree().deleteItem(m_curItem);
        context->spatialContext().quadtree().insertItem(m_curItem);

        context->spatialContext().cacheGrid().markAllDirty();
        context->renderingContext().markForRender();
        context->renderingContext().markForUpdate();
    }
}

void TextTool::keyReleased(ApplicationContext *context) {
}

ToolID TextTool::id() const {
    return ToolID::TextTool;
}

QString TextTool::iconAlt() const {
    return "󰊄";
}

#include "texttool.h"

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

        if (m_curItem != nullptr) {
            m_curItem->setMode(Text::NORMAL);
        }

        if (intersectingItems.empty()) {
            m_curItem = std::dynamic_pointer_cast<Text>(m_itemFactory->create());
            m_curItem->setBoundingBoxPadding(10 * renderingContext.canvas().scale());
            m_curItem->createTextBox(transformer.viewToWorld(uiContext.event().pos()));
            commandHistory.insert(
                std::make_shared<InsertItemCommand>(QVector<std::shared_ptr<Item>>{m_curItem}));
        } else {
            m_curItem = std::dynamic_pointer_cast<Text>(intersectingItems.back());
            m_curItem->setCaret(worldPos);
            spatialContext.cacheGrid().markDirty(
                transformer.worldToGrid(m_curItem->boundingBox()).toRect());
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
};

void TextTool::mouseReleased(ApplicationContext *context) {};

void TextTool::keyPressed(ApplicationContext *context) {
    if (m_curItem != nullptr && m_curItem->mode() == Text::EDIT) {
        Event &ev{context->uiContext().event()};
        qsizetype caret{m_curItem->caret()};
        qsizetype size{m_curItem->text().size()};

        switch (ev.key()) {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                ev.setText("\n");
                break;
            case Qt::Key_Left:
                m_curItem->setCaret(std::max(static_cast<qsizetype>(0), caret - 1));
                break;
            case Qt::Key_Right: {
                m_curItem->setCaret(std::min(size, caret + 1));
                break;
            }
            case Qt::Key_Backspace: {
                caret--;
                m_curItem->deleteSubStr(caret, caret);
                m_curItem->setCaret(caret);
                ev.setText("");
                break;
            }
            case Qt::Key_Up: {
                qsizetype prevLineEnd{m_curItem->text().lastIndexOf("\n", caret - 1)};

                if (prevLineEnd == -1)
                    break;

                qsizetype prevLineStart{m_curItem->text().lastIndexOf("\n", prevLineEnd - 1) + 1};
                qsizetype pos{m_curItem->caretPosInLine()};

                qsizetype length{prevLineEnd - prevLineStart + 1};
                if (length >= pos) {
                    m_curItem->setCaret(prevLineStart + pos - 1, false);
                } else {
                    m_curItem->setCaret(prevLineEnd, false);
                }
                ev.setText("");
                break;
            }
            case Qt::Key_Down: {
                qsizetype nextLineStart{m_curItem->text().indexOf("\n", caret)};

                if (nextLineStart == -1)
                    break;

                qsizetype nextLineEnd{m_curItem->text().indexOf("\n", ++nextLineStart)};
                if (nextLineEnd == -1)
                    nextLineEnd = m_curItem->text().size();

                qsizetype pos{m_curItem->caretPosInLine()};

                qsizetype length{nextLineEnd - nextLineStart + 1};
                if (length >= pos) {
                    m_curItem->setCaret(nextLineStart + pos - 1, false);
                } else {
                    m_curItem->setCaret(nextLineEnd, false);
                }
                ev.setText("");
                break;
            }
        }

        m_curItem->insertText(ev.text(), context->spatialContext().offsetPos());
        context->spatialContext().quadtree().deleteItem(m_curItem);
        context->spatialContext().quadtree().insertItem(m_curItem);

        context->spatialContext().cacheGrid().markAllDirty();  // TODO: Remove this line
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

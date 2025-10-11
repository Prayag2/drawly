#include "texttool.h"
#include "../item/factory/textfactory.h"
#include "../canvas/canvas.h"
#include "../context/applicationcontext.h"
#include "../context/spatialcontext.h"
#include "../context/coordinatetransformer.h"
#include "../context/renderingcontext.h"
#include "../context/uicontext.h"
#include "../command/commandhistory.h"
#include "../command/insertitemcommand.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"
#include "../keybindings/keybindmanager.h"
#include "../event/event.h"

TextTool::TextTool(const PropertyManager& propertyManager) {
    m_cursor = QCursor(Qt::IBeamCursor);
    m_itemFactory = std::make_unique<TextFactory>();
}

void TextTool::mousePressed(ApplicationContext* context) {
    UIContext& uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext& spatialContext{context->spatialContext()};
        CoordinateTransformer& transformer{spatialContext.coordinateTransformer()};
        RenderingContext& renderingContext{context->renderingContext()};
        QuadTree& quadTree{spatialContext.quadtree()};
        CommandHistory& commandHistory{spatialContext.commandHistory()};

        m_curItem = std::dynamic_pointer_cast<Text>(m_itemFactory->create());

        m_curItem->setBoundingBoxPadding(10 * renderingContext.canvas().scale());
        m_curItem->createTextBox(transformer.viewToWorld(uiContext.event().pos()));

        commandHistory.insert(std::make_shared<InsertItemCommand>(QVector<std::shared_ptr<Item>>{m_curItem}));

        m_curItem->setMode(Text::EDIT);
        uiContext.keybindManager().disable();

        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }

    m_isDrawing = true;
};

void TextTool::mouseMoved(ApplicationContext* context) {
};

void TextTool::mouseReleased(ApplicationContext* context) {
};

void TextTool::keyPressed(ApplicationContext* context) {
    if (m_curItem != nullptr && m_curItem->mode() == Text::EDIT) {
        Event& ev {context->uiContext().event()};

        switch(ev.key()) {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                ev.setText("\n");
                break;
            case Qt::Key_Left:
                m_curItem->setCursor(std::max(0, m_curItem->cursor() - 1));
                break;
            case Qt::Key_Right: {
                int size = m_curItem->text().size();
                m_curItem->setCursor(std::min(size, m_curItem->cursor() + 1));
                break;
            }
            case Qt::Key_Backspace: {
                int cursor{std::max(0, m_curItem->cursor() - 1)};
                m_curItem->deleteSubStr(cursor, cursor);
                m_curItem->setCursor(cursor);
                ev.setText("");
                break;
            }
        }

        m_curItem->insertText(ev.text(), context->spatialContext().offsetPos());

        context->spatialContext().cacheGrid().markAllDirty(); // TODO: Remove this line
        context->renderingContext().markForRender();
        context->renderingContext().markForUpdate();
    }
}

void TextTool::keyReleased(ApplicationContext* context) {

}

ToolID TextTool::id() const {
    return ToolID::TextTool;
}

QString TextTool::iconAlt() const {
    return "󰊄";
}


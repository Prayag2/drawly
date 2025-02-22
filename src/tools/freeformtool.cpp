#include "freeformtool.h"
#include "../item/factory/freeformfactory.h"
#include "../context/applicationcontext.h"
#include "../item/freeform.h"

FreeformTool::FreeformTool() {
    m_itemFactory = std::make_unique<FreeformFactory>();
    m_cursor = QCursor(Qt::CrossCursor);
}

QString FreeformTool::iconAlt() const {
    return "󰽉";
};

void FreeformTool::mousePressed(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton) {
        curItem = std::dynamic_pointer_cast<Freeform>(m_itemFactory->create());
        curItem->setBoundingBoxPadding(10*context->canvas().scale());
        curItem->addPoint(context->event().pos()-context->offsetPos());
        m_isDrawing = true;
    }
}

void FreeformTool::mouseMoved(ApplicationContext *context) {
    if (m_isDrawing) {
        QPainter& painter {context->overlayPainter()};
        painter.setPen(context->pen());

        curItem->addPoint(context->event().pos()-context->offsetPos());
        curItem->quickDraw(painter, context->offsetPos());

        context->canvas().update();
    }
}

void FreeformTool::mouseReleased(ApplicationContext *context) {
    if (context->event().button() == Qt::LeftButton && m_isDrawing) {
        QPainter& overlayPainter {context->overlayPainter()};
        QPainter& canvasPainter {context->canvasPainter()};

        overlayPainter.setCompositionMode(QPainter::CompositionMode_Clear);
        overlayPainter.fillRect(context->canvas().overlay()->rect(), Qt::transparent);
        overlayPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        canvasPainter.setPen(context->pen());
        curItem->draw(canvasPainter, context->offsetPos());
        context->quadtree().insertItem(curItem);

        m_isDrawing = false;
        qDebug() << "QuadTree size: " << context->quadtree().size();
        context->canvas().update();
    }
}

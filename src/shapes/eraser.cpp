#include "eraser.h"

Eraser::Eraser(): Shape(0) {}

DrawingMode Eraser::drawingMode() const {
    return DrawingMode::Erase;
}

const QRect& Eraser::boundingBox() const {
    return m_boundingBox;
}

ShapeType Eraser::shapeType() const {
    return ShapeType::Eraser;
}

void Eraser::setBoundingBox(QRect boundingBox) {
    m_boundingBox = boundingBox;
}

bool Eraser::intersects(QRect boundingBox) const {
    return m_boundingBox.intersects(boundingBox);
}

QCursor Eraser::createCursor(int size, int borderWidth) const {
    QBitmap eraserShape {size, size};
    QPen eraserPen {};
    eraserPen.setWidth(borderWidth);
    eraserPen.setColor(Qt::black);
    eraserPen.setJoinStyle(Qt::MiterJoin);

    QPainter eraserPainter {&eraserShape};
    eraserPainter.setPen(eraserPen);

    eraserPainter.drawRect(borderWidth/2, borderWidth/2, size-borderWidth, size-borderWidth);
    return QCursor(eraserShape, 0, 0);
}

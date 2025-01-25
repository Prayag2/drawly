#include "canvas.h"

#include <QBuffer>
#include <QBitmap>
#include "../shapes/shape.h"
#include "../shapes/stroke.h"
#include "../shapes/eraser.h"
#include "../shapes/draggableshape.h"
#include <cstdlib>

bool isWayland() {
    const char* waylandDisplay = std::getenv("WAYLAND_DISPLAY");
    return (waylandDisplay != nullptr);
}

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if (isWayland()) {
        qWarning() << "Wayland detected. Please manually set the scale since Qt's devicePixelRatioF is broken on Wayland.";
        m_scale = 1.25;
    } else {
        m_scale = this->devicePixelRatioF();
    }

    m_initialScreenSize = this->screen()->size();
    m_width = m_initialScreenSize.width();
    m_height = m_initialScreenSize.height();

    m_shapeTree = new QuadTree(QRect(m_topLeftPosition.x(), m_topLeftPosition.y(), m_width*m_scale, m_height*m_scale), 100);

    m_canvas = m_createImage(Qt::white);
    m_overlay = m_createImage(Qt::transparent);

    m_pen.setWidth(5*m_scale);
    m_pen.setColor(Qt::black);
    m_pen.setJoinStyle(Qt::MiterJoin);
    m_pen.setCapStyle(Qt::RoundCap);

    m_overlayResetPen.setWidth(std::max(5, m_pen.width()*2));
    m_overlayResetPen.setColor(Qt::transparent);
    m_overlayResetPen.setJoinStyle(Qt::MiterJoin);
    m_overlayResetPen.setCapStyle(Qt::RoundCap);

    m_selectionPen.setWidth(1*m_scale);
    m_selectionPen.setStyle(Qt::DashLine);
    m_selectionPen.setColor(Qt::blue);
    m_selectionPen.setCapStyle(Qt::RoundCap);
}

ShapeType Canvas::shape() const {
    return m_curShapeType;
}

void Canvas::paintEvent(QPaintEvent* paintEvent) {
    m_widgetPainter.begin(this);
    m_widgetPainter.scale(1.0/m_scale, 1.0/m_scale);
    m_widgetPainter.drawImage(0,0, *m_canvas);
    m_widgetPainter.drawImage(0,0, *m_overlay);
    m_widgetPainter.end();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_startCanvasPainter();
        m_startOverlayPainter();
        m_curDrawnShape = Shape::createShape(shape(), m_pen.width());

        switch(m_curDrawnShape->drawingMode()) {
        case DrawingMode::Drag:
            dynamic_cast<DraggableShape*>(m_curDrawnShape)->setStart(event->pos()*m_scale);
            break;
        case DrawingMode::Sketch:
            dynamic_cast<Stroke*>(m_curDrawnShape)->addPoint(event->pos()*m_scale);
            break;
        case DrawingMode::Erase:
            m_eraser = Eraser::createCursor(16, 2);
            this->setCursor(m_eraser);
        }

        m_isDrawing = true;
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDrawing) {
        switch(m_curDrawnShape->drawingMode()) {
        case DrawingMode::Drag:
        {
            Shape *prevShape {m_curDrawnShape};

            m_overlayPainter.setPen(m_overlayResetPen);
            m_overlayPainter.setCompositionMode(QPainter::CompositionMode_Clear);
            dynamic_cast<DraggableShape*>(prevShape)->draw(m_overlayPainter);
            m_overlayPainter.setCompositionMode(QPainter::QPainter::CompositionMode_SourceOver);

            dynamic_cast<DraggableShape*>(prevShape)->setEnd(event->pos()*m_scale);

            if (m_curDrawnShape->shapeType() == ShapeType::Selection) {
                m_overlayPainter.setPen(m_selectionPen);
                QVector<Shape*> selectedShapes {m_shapeTree->queryShapes(m_curDrawnShape->boundingBox())};
                for (Shape* const s : selectedShapes) {
                    m_overlayPainter.drawRect(s->boundingBox());
                }
            } else {
                m_overlayPainter.setPen(m_pen);
            }

            m_curDrawnShape->draw(m_overlayPainter);
            break;
        }
        case DrawingMode::Sketch:
            dynamic_cast<Stroke*>(m_curDrawnShape)->addPoint(event->pos()*m_scale);
            m_curDrawnShape->draw(m_overlayPainter);
            break;
        case DrawingMode::Erase:
            QRect eraserBoundingBox {QRect(event->pos()*m_scale, m_eraser.pixmap().size())};
            QVector<Shape*> toBeErased {m_shapeTree->queryShapes(eraserBoundingBox)};
            QVector<QRect> boundingBoxes {};

            for (Shape *s : toBeErased) {
                m_canvasPainter.fillRect(s->boundingBox(), Qt::white);
                boundingBoxes.push_back(s->boundingBox());
                m_shapeTree->deleteShape(s);
            }

            QVector<Shape*> dirtyShapes {};
            for (const QRect& box : boundingBoxes) {
                dirtyShapes += m_shapeTree->queryShapes(box, true);
            }

            for (Shape *s : dirtyShapes) {
                s->draw(m_canvasPainter);
            }
        }
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = false;
        m_overlay->fill(Qt::transparent);

        switch(m_curDrawnShape->drawingMode()) {
        case DrawingMode::Drag:
            if (m_curDrawnShape->shapeType() != ShapeType::Selection) {
                m_curDrawnShape->draw(m_canvasPainter);
                m_shapeTree->insertShape(m_curDrawnShape);
            }
            break;
        case DrawingMode::Sketch:
            m_curDrawnShape->draw(m_canvasPainter);
            m_shapeTree->insertShape(m_curDrawnShape);
            break;
        case DrawingMode::Erase:
            this->setCursor(QCursor(Qt::ArrowCursor));
        }

        m_curDrawnShape = nullptr;
        m_endCanvasPainter();
        m_endOverlayPainter();
        update();
    }
}

void Canvas::setShape(ShapeType shape) {
    m_curShapeType = shape;
}

QImage* Canvas::m_createImage(QColor fill = Qt::white) const {
    QImage *img = new QImage(m_width*m_scale, m_height*m_scale, QImage::Format_ARGB32);
    img->fill(fill);
    return img;
}

void Canvas::resizeEvent(QResizeEvent *event) {
    m_width = size().width();
    m_height = size().height();

    if (m_width <= m_initialScreenSize.width()) return;
    if (m_height <= m_initialScreenSize.height()) return;

    QByteArray oldData {};
    QBuffer buffer {&oldData};
    buffer.open(QIODevice::WriteOnly);
    m_canvas->save(&buffer, "PNG");

    m_endCanvasPainter();
    m_endOverlayPainter();

    // QVector<Shape*> shapes {m_shapeTree->getAllShapes()};
    delete m_canvas;
    delete m_overlay;

    m_canvas = m_createImage(Qt::white);
    m_overlay = m_createImage(Qt::transparent);
    m_canvas->loadFromData(oldData, "PNG");

    m_startCanvasPainter();
    m_startOverlayPainter();

    // for (Shape* const s : shapes) {
    //     s->draw(m_canvasPainter);
    // }
    QWidget::resizeEvent(event);
}

void Canvas::m_startCanvasPainter() {
    if (m_canvasPainter.isActive()) return;
    m_canvasPainter.begin(m_canvas);
    m_canvasPainter.setRenderHints(QPainter::Antialiasing, true);
    m_canvasPainter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    m_canvasPainter.setPen(m_pen);
}

void Canvas::m_startOverlayPainter() {
    if (m_overlayPainter.isActive()) return;
    m_overlayPainter.begin(m_overlay);
    m_overlayPainter.setRenderHints(QPainter::Antialiasing, true);
    m_overlayPainter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    m_overlayPainter.setPen(m_pen);
}

void Canvas::m_endCanvasPainter() {
    if (!m_canvasPainter.isActive()) return;
    m_canvasPainter.end();
}

void Canvas::m_endOverlayPainter() {
    if (!m_overlayPainter.isActive()) return;
    m_overlayPainter.end();
}

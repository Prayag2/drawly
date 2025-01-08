#include "canvas.h"

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
    parent->installEventFilter(this);
    this->setGeometry(0, 0, m_width, m_height);

    if (isWayland()) {
        qWarning() << "Wayland detected. Please manually set the scale since Qt's devicePixelRatioF is broken on Wayland.";
        m_scale = 1.25;
    } else {
        m_scale = this->devicePixelRatioF();
    }

    m_canvas = new QImage (m_width*m_scale, m_width*m_scale, QImage::Format_ARGB32);
    m_canvas->fill(Qt::white);

    m_overlayCanvas = new QImage (m_width*m_scale, m_width*m_scale, QImage::Format_ARGB32);
    m_overlayCanvas->fill(Qt::transparent);

    m_pen.setWidth(5*m_scale);
    m_pen.setColor(Qt::black);
    m_pen.setCapStyle(Qt::RoundCap);
    m_overlayResetPen.setWidth(std::max(5, m_pen.width()*2));
    m_overlayResetPen.setColor(Qt::transparent);
    m_overlayResetPen.setCapStyle(Qt::RoundCap);

    m_canvasPainter.begin(m_canvas);
    m_canvasPainter.setRenderHints(QPainter::Antialiasing, true);
    m_canvasPainter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    m_canvasPainter.setPen(m_pen);
    m_shapes = new QuadTree(QRect(0, 0, m_width*m_scale, m_width*m_scale), 100);

    m_overlayPainter.begin(m_overlayCanvas);
    m_overlayPainter.setRenderHints(QPainter::Antialiasing, true);
    m_overlayPainter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    m_overlayPainter.setPen(m_pen);

}

ShapeType Canvas::shape() const {
    return m_shape;
}

void Canvas::paintEvent(QPaintEvent* paintEvent) {
    m_widgetPainter.begin(this);
    m_widgetPainter.scale(1.0/m_scale, 1.0/m_scale);
    m_widgetPainter.drawImage(0,0, *m_canvas);
    m_widgetPainter.drawImage(0,0, *m_overlayCanvas);
    m_widgetPainter.end();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_curShape = Shape::createShape(shape(), m_pen.width());

        switch(m_curShape->drawingMode()) {
        case DrawingMode::Drag:
            dynamic_cast<DraggableShape*>(m_curShape)->setStart(event->pos()*m_scale);
            break;
        case DrawingMode::Sketch:
            dynamic_cast<Stroke*>(m_curShape)->addPoint(m_overlayPainter, event->pos()*m_scale);
            break;
        case DrawingMode::Erase:
            m_eraser = dynamic_cast<Eraser*>(m_curShape)->createCursor(16, 2);
            this->setCursor(m_eraser);
        }

        m_isDrawing = true;
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDrawing) {
        switch(m_curShape->drawingMode()) {
        case DrawingMode::Drag:
        {
            Shape *prevShape {m_curShape};

            // erasing the previous shape
            m_overlayPainter.setCompositionMode(QPainter::CompositionMode_Clear);
            m_overlayPainter.setPen(m_overlayResetPen);
            dynamic_cast<DraggableShape*>(prevShape)->draw(m_overlayPainter);
            m_overlayPainter.setPen(m_pen);
            m_overlayPainter.setCompositionMode(QPainter::QPainter::CompositionMode_SourceOver);

            dynamic_cast<DraggableShape*>(prevShape)->setEnd(event->pos()*m_scale);
            dynamic_cast<DraggableShape*>(m_curShape)->draw(m_overlayPainter);
            break;
        }
        case DrawingMode::Sketch:
            dynamic_cast<Stroke*>(m_curShape)->addPoint(m_overlayPainter, event->pos()*m_scale);
            break;
        case DrawingMode::Erase:
            QRect eraserBoundingBox {QRect(event->pos()*m_scale, m_eraser.pixmap().size())};
            QVector<Shape*> toBeErased {m_shapes->queryShapes(eraserBoundingBox)};
            QVector<QRect> boundingBoxes {};

            for (Shape *s : toBeErased) {
                m_canvasPainter.fillRect(s->boundingBox(), Qt::white);
                boundingBoxes.push_back(s->boundingBox());
                m_shapes->deleteShape(s);
            }

            QVector<Shape*> dirtyShapes {};
            for (const QRect& box : boundingBoxes) {
                dirtyShapes += m_shapes->queryShapes(box, true);
            }

            for (Shape *s : dirtyShapes) {
                if (s->drawingMode() == DrawingMode::Drag) {
                    dynamic_cast<DraggableShape*>(s)->draw(m_canvasPainter);
                } else if (s->drawingMode() == DrawingMode::Sketch) {
                    dynamic_cast<Stroke*>(s)->draw(m_canvasPainter);
                }
            }
        }
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = false;
        m_overlayCanvas->fill(Qt::transparent);
        switch(m_curShape->drawingMode()) {
        case DrawingMode::Drag:
            dynamic_cast<DraggableShape*>(m_curShape)->draw(m_canvasPainter);
            m_shapes->insertShape(m_curShape);
            break;
        case DrawingMode::Sketch:
            dynamic_cast<Stroke*>(m_curShape)->draw(m_canvasPainter);
            m_shapes->insertShape(m_curShape);
            break;
        case DrawingMode::Erase:
            this->setCursor(QCursor(Qt::ArrowCursor));
        }

        m_curShape = nullptr;
        update();
    }
}

bool Canvas::eventFilter(QObject *object, QEvent *event) {
    return QObject::eventFilter(object, event);
}

void Canvas::setShape(ShapeType shape) {
    m_shape = shape;
}

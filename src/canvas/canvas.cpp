#include "canvas.h"

#include <QBuffer>
#include <QResizeEvent>
#include <QScreen>

// PUBLIC
Canvas::Canvas(QWidget* parent) : QWidget{parent} {
    m_sizeHint = screen()->size() * m_scale;
    m_maxSize = m_sizeHint;
    setMouseTracking(true);
    m_canvas = new QImage(m_sizeHint, m_imageFormat);
    m_overlay = new QImage(m_sizeHint, m_imageFormat);
    setBg(QColor{18, 18, 18});
}

Canvas::~Canvas() {
    emit destroyed();

    delete m_canvas;
    delete m_overlay;
}

QSize Canvas::sizeHint() const {
    return m_sizeHint;
}

QImage* const Canvas::canvas() const {
    return m_canvas;
}

QImage* const Canvas::overlay() const {
    return m_overlay;
}

QColor Canvas::bg() const {
    return m_bg;
};

void Canvas::setBg(const QColor& color, QImage* canvas, QImage* overlay) {
    m_bg = color;
    if (canvas)
        canvas->fill(color);
    else
        m_canvas->fill(color);

    if (overlay)
        overlay->fill(Qt::transparent);
    else
        m_overlay->fill(Qt::transparent);
}

qreal Canvas::scale() const {
    return m_scale;
}

void Canvas::setScale(const qreal scale) {
    if (scale == 0 || m_scale == 0) return;

    m_sizeHint = (m_sizeHint * scale) / m_scale;

    if (scale > m_scale) m_maxSize = m_sizeHint;
    m_scale = scale;
    resize();
}

QSize Canvas::dimensions() const {
    return size() * m_scale;
}

// PROTECTED
void Canvas::paintEvent(QPaintEvent* event) {
    QPainter painter{this};
    painter.scale(1.0 / m_scale, 1.0 / m_scale);

    if (m_canvas) painter.drawImage(0, 0, *m_canvas);
    if (m_overlay) painter.drawImage(0, 0, *m_overlay);
}

// just a small overload
bool operator<=(const QSize& a, const QSize& b) {
    return a.height() <= b.height() && a.width() <= b.width();
}

void Canvas::resizeEvent(QResizeEvent* event) {
    emit resizeEventCalled();

    if (size() * m_scale <= m_maxSize) {
        return;
    }

    resize();

    QWidget::resizeEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent* event) {
    emit mousePressed(event);
    QWidget::mousePressEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent* event) {
    emit mouseMoved(event);
    QWidget::mouseMoveEvent(event);
};

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
    emit mouseReleased(event);
    QWidget::mouseReleaseEvent(event);
};

void Canvas::wheelEvent(QWheelEvent* event) {
    emit wheel(event);
    QWidget::wheelEvent(event);
}

// PRIVATE
QByteArray Canvas::imageData(QImage* const img) {
    QByteArray arr{};
    QBuffer buffer{&arr};
    buffer.open(QBuffer::WriteOnly);
    img->save(&buffer, "PNG");
    return arr;
}

void Canvas::setImageData(QImage* const img, const QByteArray& arr) {
    img->loadFromData(arr, "PNG");
}

void Canvas::resize() {
    emit resizeStart();

    if (m_canvas->paintingActive() || m_overlay->paintingActive()) {
        return;
    }

    QSize oldSize{m_canvas->size()};
    QSize newSize{size() * m_scale};
    m_maxSize.setWidth(std::max(oldSize.width(), newSize.width()));
    m_maxSize.setHeight(std::max(oldSize.height(), newSize.height()));

    QImage* canvas{new QImage(m_maxSize, m_imageFormat)};
    QImage* overlay{new QImage(m_maxSize, m_imageFormat)};
    setBg(bg(), canvas, overlay);

    QPainter canvasPainter{canvas}, overlayPainter{overlay};
    canvasPainter.drawImage(0, 0, *m_canvas);
    overlayPainter.drawImage(0, 0, *m_overlay);

    delete m_canvas;
    delete m_overlay;

    m_canvas = canvas;
    m_overlay = overlay;

    canvasPainter.end();
    overlayPainter.end();
    emit resizeEnd();
}

#include "canvas.h"
#include <QBuffer>
#include <QScreen>
#include <QResizeEvent>

// PUBLIC
Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
{
    m_sizeHint = screen()->size();
    setMouseTracking(true);
    m_canvas = new QImage(m_sizeHint, m_imageFormat);
    m_overlay = new QImage(m_sizeHint, m_imageFormat);
    setBg(Qt::white);
}

Canvas::~Canvas() {
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

void Canvas::setBg(const QColor& color) {
    m_bg = color;
    m_canvas->fill(color);
    m_overlay->fill(Qt::transparent);
}

// PROTECTED
void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter {this};
    if (m_canvas) painter.drawImage(0, 0, *m_canvas);
    if (m_overlay) painter.drawImage(0, 0, *m_overlay);
}

void Canvas::resizeEvent(QResizeEvent *event) {
    if (
        size().height() <= m_sizeHint.height() ||
        size().width() <= m_sizeHint.width()
    ) return;

    QByteArray oldCanvasData {getImageData(m_canvas)};
    QByteArray oldOverlayData {getImageData(m_canvas)};

    delete m_canvas;
    delete m_overlay;

    m_canvas = new QImage(size(), m_imageFormat);
    m_overlay = new QImage(size(), m_imageFormat);

    setBg(bg());
    setImageData(m_canvas, oldCanvasData);
    setImageData(m_overlay, oldOverlayData);
    QWidget::resizeEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    emit mousePressed(event);
    QWidget::mousePressEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    emit mouseMoved(event);
    QWidget::mouseMoveEvent(event);
};

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseReleased(event);
    QWidget::mouseReleaseEvent(event);
};

// PRIVATE
QByteArray Canvas::getImageData(QImage *const img) {
    QByteArray arr {};
    QBuffer buffer {&arr};
    buffer.open(QBuffer::WriteOnly);
    img->save(&buffer, "PNG");
    return arr;
}

void Canvas::setImageData(QImage *const img, const QByteArray& arr) {
    img->loadFromData(arr, "PNG");
}

#include <QScreen>
#include "renderingcontext.h"
#include "applicationcontext.h"
#include "spatialcontext.h"
#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../data-structures/cachegrid.h"

RenderingContext::RenderingContext(ApplicationContext* context) 
    : QObject{context}, m_applicationContext(context) {}

void RenderingContext::setRenderingContext() {
    m_canvas = new Canvas(m_applicationContext->parentWidget());

    m_canvasPainter = new QPainter(m_canvas->canvas());
    m_overlayPainter = new QPainter(m_canvas->overlay());

    QObject::connect(m_canvas, &Canvas::destroyed, this, &RenderingContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeStart, this, &RenderingContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeEnd, this, &RenderingContext::beginPainters);
    QObject::connect(m_canvas, &Canvas::resizeEventCalled, this, &RenderingContext::canvasResized);

    QObject::connect(&m_frameTimer, &QTimer::timeout, m_canvas, [&]() {
        if (m_needsReRender) {
            qDebug() << "Rendering";
            Common::renderCanvas(m_applicationContext);
            m_needsReRender = false;
        }

        if (m_needsUpdate) {
            qDebug() << "Updating";
            if (m_updateRegion.width() == 0 || m_updateRegion.height() == 0) {
                m_canvas->update();
            } else {
                m_canvas->update(m_updateRegion);
            }

            m_updateRegion.setWidth(0);
            m_updateRegion.setHeight(0);
            m_needsUpdate = false;
        }
    });

    m_frameTimer.start(1000 / fps());
}

RenderingContext::~RenderingContext() {
    delete m_canvasPainter;
}

Canvas& RenderingContext::canvas() const {
    return *m_canvas;
}

QPainter& RenderingContext::canvasPainter() const {
    return *m_canvasPainter;
}

QPainter& RenderingContext::overlayPainter() const {
    return *m_overlayPainter;
}

// PRIVATE SLOTS
void RenderingContext::endPainters() {
    if (m_canvasPainter->isActive()) m_canvasPainter->end();
    if (m_overlayPainter->isActive()) m_overlayPainter->end();
}

void RenderingContext::beginPainters() {
    QPainter::RenderHints renderHints{QPainter::Antialiasing | QPainter::SmoothPixmapTransform};
    if (!m_canvasPainter->isActive()) {
        m_canvasPainter->begin(m_canvas->canvas());
        m_canvasPainter->setRenderHints(renderHints);
    }
    if (!m_overlayPainter->isActive()) {
        m_overlayPainter->begin(m_canvas->overlay());
        m_overlayPainter->setRenderHints(renderHints);
    }
}

qreal RenderingContext::zoomFactor() const {
    return m_zoomFactor;
}

void RenderingContext::setZoomFactor(int diff) {
    // zoom out limit is 0.1
    if (diff < 0 && m_zoomFactor - 0.1 <= 1e-9) return;

    qreal oldZoomFactor = m_zoomFactor;
    m_zoomFactor += diff * 0.1;

    qDebug() << "Zoom: " << m_zoomFactor;

    QSize viewport{canvas().dimensions()};
    QPointF offsetPos{m_applicationContext->spatialContext().offsetPos()};

    offsetPos.setX(offsetPos.x() + viewport.width() / (2 * oldZoomFactor) -
                     viewport.width() / (2 * m_zoomFactor));
    offsetPos.setY(offsetPos.y() + viewport.height() / (2 * oldZoomFactor) -
                     viewport.height() / (2 * m_zoomFactor));

    // changes scale
    endPainters();
    beginPainters();

    m_applicationContext->spatialContext().cacheGrid().markAllDirty();
    Common::renderCanvas(m_applicationContext);

    canvas().update();
}

const int RenderingContext::fps() const {
    QScreen* screen{m_canvas->screen()};
    if (screen) {
        return screen->refreshRate();
    }

    return 60;
}

void RenderingContext::canvasResized() {
    int width{m_canvas->dimensions().width()}, height{m_canvas->dimensions().height()};
    int cellW{CacheCell::cellSize().width()}, cellH{CacheCell::cellSize().height()};
    int rows{static_cast<int>(std::ceil(height / static_cast<double>(cellH)) + 1)};
    int cols{static_cast<int>(std::ceil(width / static_cast<double>(cellW)) + 1)};

    m_applicationContext->spatialContext().cacheGrid().setSize(9 * rows * cols);
}

void RenderingContext::markForRender() {
    m_needsReRender = true;
}

void RenderingContext::markForUpdate() {
    m_needsUpdate = true;
}

void RenderingContext::markForUpdate(const QRect& region) {
    m_needsUpdate = true;
    m_updateRegion = region;
}

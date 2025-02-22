#include <QRect>
#include "applicationcontext.h"
#include "../tools/rectangletool.h"
#include "../tools/ellipsetool.h"
#include "../tools/linetool.h"
#include "../tools/arrowtool.h"
#include "../tools/freeformtool.h"
#include "../tools/erasertool.h"
#include "../tools/movetool.h"

ApplicationContext::ApplicationContext(QWidget* parent)
    : QObject {parent} {
    m_canvas = new Canvas(parent);
    m_canvas->setScale(1.25);

    m_toolbar = new Toolbar(parent);
    m_pen = new QPen();
    m_quadtree = new QuadTree(QRect{{0, 0}, m_canvas->sizeHint()}, 100);
    m_event = new Event();

    m_canvasPainter = new QPainter(m_canvas->canvas());
    m_overlayPainter = new QPainter(m_canvas->overlay());

    m_toolbar->addTool(new RectangleTool());
    m_toolbar->addTool(new EllipseTool());
    m_toolbar->addTool(new ArrowTool());
    m_toolbar->addTool(new LineTool());
    m_toolbar->addTool(new FreeformTool());
    m_toolbar->addTool(new EraserTool());
    m_toolbar->addTool(new MoveTool());

    QObject::connect(m_canvas, &Canvas::destroyed, this, &ApplicationContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeStart, this, &ApplicationContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeEnd, this, &ApplicationContext::beginPainters);
    QObject::connect(m_toolbar, &Toolbar::toolChanged, this, &ApplicationContext::toolChanged);

    m_canvasPainter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_overlayPainter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    pen().setCapStyle(Qt::RoundCap);
    pen().setJoinStyle(Qt::RoundJoin);
    pen().setWidth(2);
    pen().setColor(Qt::black);
}

ApplicationContext::~ApplicationContext() {
    delete m_pen;
    delete m_quadtree;
    delete m_event;
    delete m_canvasPainter;
    delete m_overlayPainter;
}

Canvas& ApplicationContext::canvas() const {
    return *m_canvas;
}

QuadTree& ApplicationContext::quadtree() const {
    return *m_quadtree;
}

Toolbar& ApplicationContext::toolbar() const {
    return *m_toolbar;
}

QPen& ApplicationContext::pen() const {
    return *m_pen;
}

Event& ApplicationContext::event() const {
    return *m_event;
}

QPainter& ApplicationContext::canvasPainter() const {
    return *m_canvasPainter;
}

QPainter& ApplicationContext::overlayPainter() const {
    return *m_overlayPainter;
}

// PRIVATE SLOTS
void ApplicationContext::endPainters() {
    if (m_canvasPainter->isActive()) m_canvasPainter->end();
    if (m_overlayPainter->isActive()) m_overlayPainter->end();
}

void ApplicationContext::beginPainters() {
    if (!m_canvasPainter->isActive()) {
        m_canvasPainter->begin(m_canvas->canvas());
        m_canvasPainter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }
    if (!m_overlayPainter->isActive()) {
        m_overlayPainter->begin(m_canvas->overlay());
        m_overlayPainter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }
}

void ApplicationContext::toolChanged(Tool& tool) {
    m_canvas->setCursor(tool.cursor());
}

const QPoint& ApplicationContext::offsetPos() const {
    return m_offsetPos;
}

void ApplicationContext::setOffsetPos(const QPoint& pos) {
    m_offsetPos = pos;
}

#include <QRect>
#include "applicationcontext.h"
#include "../components/propertybar.h"
#include "../components/toolbar.h"
#include "../canvas/canvas.h"
#include "../data-structures/quadtree.h"
#include "../data-structures/cachegrid.h"
#include "../event/event.h"
#include "../tools/properties/propertymanager.h"
#include "../tools/rectangletool.h"
#include "../tools/ellipsetool.h"
#include "../tools/linetool.h"
#include "../tools/arrowtool.h"
#include "../tools/freeformtool.h"
#include "../tools/erasertool.h"
#include "../tools/movetool.h"

ApplicationContext::ApplicationContext(QWidget* parent)
    : QObject {parent} {
    m_fps = 120;
    m_canvas = new Canvas(parent);
    m_canvas->setScale(1.25);

    m_toolBar = new ToolBar(parent);
    m_propertyBar = new PropertyBar(parent);
    m_propertyManager = new PropertyManager(m_propertyBar);
    m_quadtree = std::make_unique<QuadTree>(QRect{{0, 0}, m_canvas->sizeHint()}, 100);


    m_cacheGrid = std::make_unique<CacheGrid>(100);
    m_event = new Event();

    m_canvasPainter = new QPainter(m_canvas->canvas());
    m_overlayPainter = new QPainter(m_canvas->overlay());

    QObject::connect(m_canvas, &Canvas::destroyed, this, &ApplicationContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeStart, this, &ApplicationContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeEnd, this, &ApplicationContext::beginPainters);
    QObject::connect(m_toolBar, &ToolBar::toolChanged, this, &ApplicationContext::toolChanged);
    QObject::connect(m_toolBar, &ToolBar::toolChanged, m_propertyBar, &PropertyBar::toolChanged);
    QObject::connect(m_canvas, &Canvas::resizeEventCalled, this, &ApplicationContext::canvasResized);

    m_toolBar->addTool(new FreeformTool(*m_propertyManager));
    m_toolBar->addTool(new RectangleTool(*m_propertyManager));
    m_toolBar->addTool(new EllipseTool(*m_propertyManager));
    m_toolBar->addTool(new ArrowTool(*m_propertyManager));
    m_toolBar->addTool(new LineTool(*m_propertyManager));
    m_toolBar->addTool(new EraserTool());
    m_toolBar->addTool(new MoveTool());

    m_canvasPainter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_overlayPainter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    m_propertyBar->toolChanged(m_toolBar->curTool());
}

ApplicationContext::~ApplicationContext() {
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

CacheGrid& ApplicationContext::cacheGrid() const {
    return *m_cacheGrid;
}

ToolBar& ApplicationContext::toolBar() const {
    return *m_toolBar;
}

PropertyBar& ApplicationContext::propertyBar() const {
    return *m_propertyBar;
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

int ApplicationContext::fps() const {
    return m_fps;
}

void ApplicationContext::canvasResized() {
    int width {m_canvas->dimensions().width()}, height {m_canvas->dimensions().height()};
    qDebug() << "width: " << width << " height: " << height;
    int cellW {CacheCell::cellSize().width()}, cellH {CacheCell::cellSize().height()};
    int rows = std::ceil(height/static_cast<double>(cellH))+1;
    int cols = std::ceil(width/static_cast<double>(cellW))+1;

    qDebug() << "Grid Size: " << rows*cols;
    m_cacheGrid->setSize(rows*cols);
}

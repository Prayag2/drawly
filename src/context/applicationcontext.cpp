#include "applicationcontext.h"

#include "../canvas/canvas.h"
#include "../common/renderitems.h"
#include "../components/actionbar.h"
#include "../components/propertybar.h"
#include "../components/toolbar.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "../item/item.h"
#include "../tools/arrowtool.h"
#include "../tools/ellipsetool.h"
#include "../tools/erasertool.h"
#include "../tools/freeformtool.h"
#include "../tools/linetool.h"
#include "../tools/movetool.h"
#include "../tools/properties/propertymanager.h"
#include "../tools/rectangletool.h"
#include "../tools/selectiontool.h"
#include "coordinatetransformer.h"
#include <QRect>
#include <QScreen>
#include <memory>

ApplicationContext::ApplicationContext(QWidget* parent) : QObject{parent} {
    m_canvas = new Canvas(parent);

    m_toolBar = new ToolBar(parent);
    m_actionBar = new ActionBar(parent);
    m_propertyBar = new PropertyBar(parent);
    m_propertyManager = new PropertyManager(m_propertyBar);
    m_quadtree = std::make_unique<QuadTree>(QRect{{0, 0}, m_canvas->sizeHint()},
                                            100);  // just an arbitrary capacity for now

    m_coordinateTransformer = std::make_unique<CoordinateTransformer>(this);
    m_cacheGrid = std::make_unique<CacheGrid>(100);
    m_event = new Event();

    m_canvasPainter = new QPainter(m_canvas->canvas());
    m_overlayPainter = new QPainter(m_canvas->overlay());

    QObject::connect(m_canvas, &Canvas::destroyed, this, &ApplicationContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeStart, this, &ApplicationContext::endPainters);
    QObject::connect(m_canvas, &Canvas::resizeEnd, this, &ApplicationContext::beginPainters);
    QObject::connect(m_toolBar, &ToolBar::toolChanged, this, &ApplicationContext::toolChanged);
    QObject::connect(m_toolBar, &ToolBar::toolChanged, m_propertyBar, &PropertyBar::toolChanged);
    QObject::connect(m_canvas, &Canvas::resizeEventCalled, this,
                     &ApplicationContext::canvasResized);

    m_toolBar->addTool(new SelectionTool());
    m_toolBar->addTool(new FreeformTool(*m_propertyManager));
    m_toolBar->addTool(new RectangleTool(*m_propertyManager));
    m_toolBar->addTool(new EllipseTool(*m_propertyManager));
    m_toolBar->addTool(new ArrowTool(*m_propertyManager));
    m_toolBar->addTool(new LineTool(*m_propertyManager));
    m_toolBar->addTool(new EraserTool());
    m_toolBar->addTool(new MoveTool());

    m_actionBar->addButton("-", 1);
    m_actionBar->addButton("+", 2);
    m_actionBar->addButton("󰖨", 3);
    QObject::connect(&m_actionBar->button(1), &QPushButton::clicked, this,
                     [this]() { setZoomFactor(-1); });
    QObject::connect(&m_actionBar->button(2), &QPushButton::clicked, this,
                     [this]() { setZoomFactor(1); });
    QObject::connect(&m_actionBar->button(3), &QPushButton::clicked, this, [this]() {
        if (m_canvas->bg() == QColor{18, 18, 18}) {
            m_canvas->setBg(QColor{233, 225, 203});
        } else {
            m_canvas->setBg(QColor{18, 18, 18});
        }
    });

    QObject::connect(&m_frameTimer, &QTimer::timeout, m_canvas, [&]() {
        if (m_needsReRender) {
            Common::renderCanvas(this);
            m_needsReRender = false;
        }

        if (m_needsUpdate) {
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

    m_propertyBar->toolChanged(m_toolBar->curTool());
}

ApplicationContext::~ApplicationContext() {
    delete m_event;
    delete m_canvasPainter;
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

ActionBar& ApplicationContext::actionBar() const {
    return *m_actionBar;
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

CoordinateTransformer& ApplicationContext::coordinateTransformer() const {
    return *m_coordinateTransformer;
}

std::unordered_set<std::shared_ptr<Item>>& ApplicationContext::selectedItems() {
    return m_selectedItems;
}

QRectF ApplicationContext::selectionBox() const {
    QRectF selectionBox;
    for (auto item : m_selectedItems) {
        selectionBox |= item->boundingBox();
    }
    return selectionBox;
}

// PRIVATE SLOTS
void ApplicationContext::endPainters() {
    if (m_canvasPainter->isActive()) m_canvasPainter->end();
    if (m_overlayPainter->isActive()) m_overlayPainter->end();
}

void ApplicationContext::beginPainters() {
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

void ApplicationContext::toolChanged(Tool& tool) {
    m_canvas->setCursor(tool.cursor());
    m_selectedItems.clear();

    Common::renderCanvas(this);
    canvas().update();
}

const QPointF& ApplicationContext::offsetPos() const {
    return m_offsetPos;
}

void ApplicationContext::setOffsetPos(const QPointF& pos) {
    m_offsetPos = pos;
}

qreal ApplicationContext::zoomFactor() const {
    return m_zoomFactor;
}

void ApplicationContext::setZoomFactor(int diff) {
    // zoom out limit is 0.1
    if (diff < 0 && m_zoomFactor - 0.1 <= 1e-9) return;

    qreal oldZoomFactor = m_zoomFactor;
    m_zoomFactor += diff * 0.1;

    qDebug() << "Zoom: " << m_zoomFactor;

    QSize viewport{canvas().dimensions()};
    m_offsetPos.setX(m_offsetPos.x() + viewport.width() / (2 * oldZoomFactor) -
                     viewport.width() / (2 * m_zoomFactor));
    m_offsetPos.setY(m_offsetPos.y() + viewport.height() / (2 * oldZoomFactor) -
                     viewport.height() / (2 * m_zoomFactor));

    // changes scale
    endPainters();
    beginPainters();

    cacheGrid().markAllDirty();
    Common::renderCanvas(this);

    canvas().update();
}

const int ApplicationContext::fps() const {
    QScreen* screen{m_canvas->screen()};
    if (screen) {
        return screen->refreshRate();
    }

    return 60;
}

void ApplicationContext::canvasResized() {
    int width{m_canvas->dimensions().width()}, height{m_canvas->dimensions().height()};
    int cellW{CacheCell::cellSize().width()}, cellH{CacheCell::cellSize().height()};
    int rows{static_cast<int>(std::ceil(height / static_cast<double>(cellH)) + 1)};
    int cols{static_cast<int>(std::ceil(width / static_cast<double>(cellW)) + 1)};

    // m_cacheGrid->setSize(9 * rows * cols);
}

void ApplicationContext::markForRender() {
    m_needsReRender = true;
}

void ApplicationContext::markForUpdate() {
    m_needsUpdate = true;
}

void ApplicationContext::markForUpdate(const QRect& region) {
    m_needsUpdate = true;
    m_updateRegion = region;
}

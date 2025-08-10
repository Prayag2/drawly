#include "uicontext.h"
#include "applicationcontext.h"
#include "selectioncontext.h"
#include "renderingcontext.h"

#include "../common/renderitems.h"
#include "../canvas/canvas.h"
#include "../event/event.h"
#include "../components/toolbar.h"
#include "../components/actionbar.h"
#include "../components/propertybar.h"
#include "../tools/properties/propertymanager.h"

#include "../tools/selectiontool/selectiontool.h"
#include "../tools/freeformtool.h"
#include "../tools/rectangletool.h"
#include "../tools/ellipsetool.h"
#include "../tools/arrowtool.h"
#include "../tools/linetool.h"
#include "../tools/erasertool.h"
#include "../tools/movetool.h"

UIContext::UIContext(ApplicationContext* context)
    : QObject{context}, m_applicationContext{context} {}

void UIContext::setUIContext() {
    m_toolBar = new ToolBar(m_applicationContext->parentWidget());
    m_actionBar = new ActionBar(m_applicationContext->parentWidget());
    m_propertyBar = new PropertyBar(m_applicationContext->parentWidget());

    m_propertyManager = new PropertyManager(m_propertyBar);
    m_event = new Event();

    m_toolBar->addTool(new SelectionTool());
    m_toolBar->addTool(new FreeformTool(*m_propertyManager));
    m_toolBar->addTool(new RectangleTool(*m_propertyManager));
    m_toolBar->addTool(new EllipseTool(*m_propertyManager));
    m_toolBar->addTool(new ArrowTool(*m_propertyManager));
    m_toolBar->addTool(new LineTool(*m_propertyManager));
    m_toolBar->addTool(new EraserTool(*m_propertyManager));
    m_toolBar->addTool(new MoveTool());

    m_actionBar->addButton("-", 1);
    m_actionBar->addButton("+", 2);
    m_actionBar->addButton("󰖨", 3);

    QObject::connect(m_toolBar, &ToolBar::toolChanged, this, &UIContext::toolChanged);
    QObject::connect(m_toolBar, &ToolBar::toolChanged, m_propertyBar, &PropertyBar::toolChanged);

    QObject::connect(&m_actionBar->button(1), &QPushButton::clicked, this,
                     [this]() { m_applicationContext->renderingContext().setZoomFactor(-1); });
    QObject::connect(&m_actionBar->button(2), &QPushButton::clicked, this,
                     [this]() { m_applicationContext->renderingContext().setZoomFactor(1); });

    QObject::connect(&m_actionBar->button(3), &QPushButton::clicked, this, [this]() {
        Canvas& canvas{m_applicationContext->renderingContext().canvas()};

        if (canvas.bg() == Common::lightBackgroundColor) {
            canvas.setBg(Common::darkBackgroundColor);
        } else {
            canvas.setBg(Common::lightBackgroundColor);
        }

        m_applicationContext->renderingContext().markForRender();
        m_applicationContext->renderingContext().markForUpdate();
    });

    m_propertyBar->toolChanged(m_toolBar->curTool());
}

UIContext::~UIContext() {
    delete m_event;
}

ToolBar& UIContext::toolBar() const {
    return *m_toolBar;
}

PropertyBar& UIContext::propertyBar() const {
    return *m_propertyBar;
}

ActionBar& UIContext::actionBar() const {
    return *m_actionBar;
}

Event& UIContext::event() const {
    return *m_event;
}

void UIContext::toolChanged(Tool& tool) {
    m_applicationContext->selectionContext().selectedItems().clear();

    Common::renderCanvas(m_applicationContext);

    Canvas& canvas{m_applicationContext->renderingContext().canvas()};
    canvas.setCursor(tool.cursor());
    canvas.update();
}

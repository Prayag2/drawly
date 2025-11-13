#include "uicontext.h"

#include "../canvas/canvas.h"
#include "../command/commandhistory.h"
#include "../common/renderitems.h"
#include "../components/actionbar.h"
#include "../components/propertybar.h"
#include "../components/toolbar.h"
#include "../event/event.h"
#include "../keybindings/actionmanager.h"
#include "../keybindings/keybindmanager.h"
#include "../tools/arrowtool.h"
#include "../tools/ellipsetool.h"
#include "../tools/erasertool.h"
#include "../tools/freeformtool.h"
#include "../tools/linetool.h"
#include "../tools/movetool.h"
#include "../properties/widgets/propertymanager.h"
#include "../tools/rectangletool.h"
#include "../tools/selectiontool/selectiontool.h"
#include "../tools/texttool.h"
#include "applicationcontext.h"
#include "renderingcontext.h"
#include "selectioncontext.h"
#include "spatialcontext.h"

UIContext::UIContext(ApplicationContext *context)
    : QObject{context},
      m_applicationContext{context} {
}

UIContext::~UIContext() {
    delete m_event;
    qDebug() << "Object deleted: UIContext";
}

void UIContext::setUIContext() {
    m_toolBar = new ToolBar(m_applicationContext->parentWidget());
    m_actionBar = new ActionBar(m_applicationContext->parentWidget());
    m_propertyBar = new PropertyBar(m_applicationContext->parentWidget());
    m_keybindManager = new KeybindManager(&m_applicationContext->renderingContext().canvas());
    m_actionManager = new ActionManager(m_applicationContext);

    m_propertyManager = new PropertyManager(m_propertyBar);
    m_propertyBar->setPropertyManager(m_propertyManager);

    QObject::connect(m_propertyManager, &PropertyManager::propertyUpdated,
                     &m_applicationContext->selectionContext(), &SelectionContext::updatePropertyOfSelectedItems);

    m_event = new Event();

    m_toolBar->addTool(new SelectionTool(), Tool::Selection);
    m_toolBar->addTool(new FreeformTool(), Tool::Freeform);
    m_toolBar->addTool(new RectangleTool(), Tool::Rectangle);
    m_toolBar->addTool(new EllipseTool(), Tool::Ellipse);
    m_toolBar->addTool(new ArrowTool(), Tool::Arrow);
    m_toolBar->addTool(new LineTool(), Tool::Line);
    m_toolBar->addTool(new EraserTool(), Tool::Eraser);
    m_toolBar->addTool(new TextTool(), Tool::Text);
    m_toolBar->addTool(new MoveTool(), Tool::Move);

    m_actionBar->addButton("-", 1);
    m_actionBar->addButton("+", 2);
    m_actionBar->addButton("󰖨", 3);
    m_actionBar->addButton("󰕌", 4);
    m_actionBar->addButton("󰑎", 5);

    QObject::connect(m_toolBar, &ToolBar::toolChanged, this, &UIContext::toolChanged);
    QObject::connect(m_toolBar, &ToolBar::toolChanged, m_propertyBar, &PropertyBar::updateProperties);

    QObject::connect(&m_actionBar->button(1), &QPushButton::clicked, this, [this]() {
        m_applicationContext->renderingContext().setZoomFactor(-1);
    });

    QObject::connect(&m_actionBar->button(2), &QPushButton::clicked, this, [this]() {
        m_applicationContext->renderingContext().setZoomFactor(1);
    });

    QObject::connect(&m_actionBar->button(4), &QPushButton::clicked, this, [this]() {
        m_applicationContext->spatialContext().commandHistory().undo();
        m_applicationContext->renderingContext().markForRender();
        m_applicationContext->renderingContext().markForUpdate();
    });

    QObject::connect(&m_actionBar->button(5), &QPushButton::clicked, this, [this]() {
        m_applicationContext->spatialContext().commandHistory().redo();
        m_applicationContext->renderingContext().markForRender();
        m_applicationContext->renderingContext().markForUpdate();
    });

    QObject::connect(&m_actionBar->button(3), &QPushButton::clicked, this, [this]() {
        Canvas &canvas{m_applicationContext->renderingContext().canvas()};

        if (canvas.bg() == Common::lightBackgroundColor) {
            canvas.setBg(Common::darkBackgroundColor);
        } else {
            canvas.setBg(Common::lightBackgroundColor);
        }

        m_applicationContext->renderingContext().markForRender();
        m_applicationContext->renderingContext().markForUpdate();
    });

    m_propertyBar->updateProperties(m_toolBar->curTool());
}

ToolBar &UIContext::toolBar() const {
    return *m_toolBar;
}

PropertyBar &UIContext::propertyBar() const {
    return *m_propertyBar;
}

ActionBar &UIContext::actionBar() const {
    return *m_actionBar;
}

KeybindManager &UIContext::keybindManager() const {
    return *m_keybindManager;
}

ActionManager &UIContext::actionManager() const {
    return *m_actionManager;
}

PropertyManager &UIContext::propertyManager() const {
    return *m_propertyManager;
}

Event &UIContext::event() const {
    return *m_event;
}

void UIContext::toolChanged(Tool &tool) {
    m_applicationContext->selectionContext().selectedItems().clear();

    Common::renderCanvas(m_applicationContext);

    Canvas &canvas{m_applicationContext->renderingContext().canvas()};

    if (m_lastTool != nullptr)
        m_lastTool->cleanup();

    m_lastTool = &tool;
    canvas.setCursor(tool.cursor());

    m_applicationContext->renderingContext().markForUpdate();
}

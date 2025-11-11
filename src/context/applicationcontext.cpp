#include "applicationcontext.h"

#include "coordinatetransformer.h"
#include "renderingcontext.h"
#include "selectioncontext.h"
#include "spatialcontext.h"
#include "uicontext.h"

ApplicationContext* ApplicationContext::m_instance = nullptr;

ApplicationContext::ApplicationContext(QWidget *parent) : QObject{parent}, m_parentWidget{parent} {
    m_selectionContext = new SelectionContext(this);
    m_renderingContext = new RenderingContext(this);
    m_uiContext = new UIContext(this);
    m_spatialContext = new SpatialContext(this);
}

ApplicationContext::~ApplicationContext() {
    qDebug() << "Object deleted: ApplicationContext";
}

void ApplicationContext::setContexts() {
    m_renderingContext->setRenderingContext();
    m_uiContext->setUIContext();
    m_spatialContext->setSpatialContext();
    m_spatialContext->coordinateTransformer().setCoordinateTransformer();
}

QWidget *ApplicationContext::parentWidget() const {
    return m_parentWidget;
}

RenderingContext &ApplicationContext::renderingContext() const {
    return *m_renderingContext;
}

SpatialContext &ApplicationContext::spatialContext() const {
    return *m_spatialContext;
}

UIContext &ApplicationContext::uiContext() const {
    return *m_uiContext;
}

SelectionContext &ApplicationContext::selectionContext() const {
    return *m_selectionContext;
}

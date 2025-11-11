#include "propertybar.h"

#include "toolbar.h"
#include "../context/applicationcontext.h"
#include "../context/uicontext.h"
#include "../properties/widgets/propertywidget.h"
#include "../properties/widgets/propertymanager.h"
#include "../tools/tool.h"
#include <QLabel>
#include <QVBoxLayout>
#include <stdexcept>

PropertyBar::PropertyBar(QWidget *parent) : QFrame{parent}
{
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    this->setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_layout = new QVBoxLayout{this};
    setLayout(m_layout);
}

void PropertyBar::setPropertyManager(PropertyManager *manager) {
    m_propertyManager = manager;
}

void PropertyBar::updateToolProperties() {
    ApplicationContext *context{ApplicationContext::instance()};
    updateProperties(context->uiContext().toolBar().curTool());
}

// PUBLIC SLOTS
void PropertyBar::updateProperties(Tool &tool) {
    // remove existing widgets
    QLayoutItem *curItem;
    while ((curItem = m_layout->takeAt(0)) != nullptr) {
        QWidget *widget = curItem->widget();
        if (widget) {
            widget->hide();
            widget->setParent(nullptr);

            // this is the property label
            if (qobject_cast<QLabel *>(widget) != nullptr) {
                widget->deleteLater();
            }
        }
        delete curItem;
    }

    QVector<Property::Type> properties{tool.properties()};
    if (properties.empty()) {
        hide();
    } else {
        show();
    }

    for (Property::Type property : properties) {
        try {
            const PropertyWidget& widget{m_propertyManager->widget(property)};
            QLabel *widgetLabel{new QLabel{widget.name(), this}};
            m_layout->addWidget(widgetLabel);
            m_layout->addWidget(widget.widget());
            widget.widget()->show();
        } catch (const std::logic_error& e) {
            // ignore this property
        }
    }

    update();
}

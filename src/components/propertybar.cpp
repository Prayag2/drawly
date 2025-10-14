#include "propertybar.h"

#include "../tools/properties/toolproperty.h"
#include "../tools/tool.h"
#include <QLabel>
#include <QVBoxLayout>

PropertyBar::PropertyBar(QWidget *parent) : QFrame{parent} {
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    this->setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_layout = new QVBoxLayout{this};
    setLayout(m_layout);
}

// PUBLIC SLOTS
void PropertyBar::toolChanged(Tool &tool) {
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

    QVector<std::shared_ptr<ToolProperty>> properties{tool.properties()};
    if (properties.empty()) {
        hide();
    } else {
        show();
    }

    for (std::shared_ptr<ToolProperty> property : properties) {
        QLabel *widgetLabel{new QLabel{property->name(), this}};
        m_layout->addWidget(widgetLabel);
        m_layout->addWidget(property->widget());
        property->widget()->show();
    }

    update();
}

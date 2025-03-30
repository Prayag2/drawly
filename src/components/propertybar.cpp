#include <QLabel>
#include "propertybar.h"
#include "../tools/tool.h"
#include "../tools/properties/toolproperty.h"

PropertyBar::PropertyBar(QWidget* parent) : QFrame{parent} {
    m_layout = new QVBoxLayout {this};
    m_layout->addSpacing(10);

    this->setLayout(m_layout);
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    this->setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

// PUBLIC SLOTS
void PropertyBar::toolChanged(Tool& tool) {
    QVector<std::shared_ptr<ToolProperty>> properties {tool.properties()};

    // removing previous widgets
    QLayoutItem *item {};
    while ((item = m_layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            if (item->widget()->property("isWidgetLabel") == "true") delete item->widget();
        }
        delete item;
    }

    for (std::shared_ptr<ToolProperty> property : properties) {
        QLabel* widgetLabel{new QLabel{property->name(), this}};
        widgetLabel->setProperty("isWidgetLabel", "true");
        m_layout->addWidget(widgetLabel);
        m_layout->addWidget(&(property->widget()));
    }

    if (properties.empty()) setVisible(false);
    else setVisible(true);

    update();
}

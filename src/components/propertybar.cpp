#include <QLabel>
#include "propertybar.h"
#include "../tools/tool.h"
#include "../tools/properties/toolproperty.h"

PropertyBar::PropertyBar(QWidget* parent) : QFrame{parent} {
    m_layout = new QVBoxLayout {this};
    this->setLayout(m_layout);
    this->setStyleSheet("QFrame { border: 2px solid black; padding: 10px; }");
}

// PUBLIC SLOTS
void PropertyBar::toolChanged(Tool& tool) {
    QVector<std::shared_ptr<ToolProperty>> properties {tool.properties()};

    // removing previous widgets
    QLayoutItem *item {};
    while ((item = m_layout->takeAt(0)) != nullptr) {
        delete item;
    }

    for (std::shared_ptr<ToolProperty> property : properties) {
        m_layout->addWidget(&(property->widget()));
    }

    update();
}

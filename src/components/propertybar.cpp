/*
* Drawy - A simple brainstorming tool with an infinite canvas
* Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "propertybar.h"

#include "../context/applicationcontext.h"
#include "../context/uicontext.h"
#include "../properties/widgets/propertymanager.h"
#include "../properties/widgets/propertywidget.h"
#include "../tools/tool.h"
#include "toolbar.h"
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <stdexcept>

PropertyBar::PropertyBar(QWidget *parent) : QFrame{parent} {
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    this->setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setProperty("class", "drawlyFrame");

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

    qsizetype count{0};
    for (Property::Type property : properties) {
        try {
            const PropertyWidget &widget{m_propertyManager->widget(property)};
            QLabel *widgetLabel{new QLabel{widget.name(), this}};
            m_layout->addWidget(widgetLabel);
            m_layout->addWidget(widget.widget());

            if (count > 0) {
                widgetLabel->setProperty("class", "drawlyPropertyLabelMargin");
            }

            count++;
            widget.widget()->show();
        } catch (const std::logic_error &e) {
            // ignore this property
        }
    }

    update();
}

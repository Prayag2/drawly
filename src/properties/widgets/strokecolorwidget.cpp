#include "strokecolorwidget.h"

#include "../property.h"
#include <QButtonGroup>
#include <QColor>
#include <QHBoxLayout>
#include <QPushButton>

// TODO: Use a better widget and a better way to return colors instead of button id
StrokeColorWidget::StrokeColorWidget(QWidget *parent) : PropertyWidget{parent} {
    m_widget = new QWidget{parent};

    QHBoxLayout *layout{new QHBoxLayout{m_widget}};
    layout->setContentsMargins(0, 0, 0, 0);

    m_group = new QButtonGroup{m_widget};
    m_widget->setLayout(layout);

    QVector<QColor> colors{QColor{255, 255, 255},
                           QColor{0, 0, 0},
                           QColor{255, 53, 71},
                           QColor{255, 187, 51},
                           QColor{51, 181, 229}};

    for (const QColor &color : colors) {
        QPushButton *btn{new QPushButton{"", m_widget}};
        btn->setCheckable(true);
        btn->setStyleSheet("background-color: " + color.name());

        layout->addWidget(btn);
        m_group->addButton(btn, color.rgba());
    }

    layout->setSpacing(0);
    m_group->buttons()[0]->setChecked(true);
    m_widget->setProperty("class", "drawlyStrokeColorWidget");
    m_widget->hide();

    QObject::connect(m_group, &QButtonGroup::idClicked, this, [this]() { emit changed(value()); });
}

QString StrokeColorWidget::name() const {
    return "Color";
};

const Property StrokeColorWidget::value() const {
    return Property{QColor::fromRgba(static_cast<quint64>(m_group->checkedId())),
                    Property::StrokeColor};
};

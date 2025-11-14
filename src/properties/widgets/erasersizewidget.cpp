#include "erasersizewidget.h"

#include "../property.h"
#include <QSpinBox>

EraserSizeWidget::EraserSizeWidget(QWidget *parent) : PropertyWidget{parent} {
    QSpinBox *box{new QSpinBox(parent)};
    box->setRange(1, 100);
    box->setValue(30);

    box->hide();
    m_widget = box;

    QObject::connect(box, &QSpinBox::valueChanged, this, [this]() { emit changed(value()); });
}

QString EraserSizeWidget::name() const {
    return "Eraser Size";
};

const Property EraserSizeWidget::value() const {
    return Property{dynamic_cast<QSpinBox *>(m_widget)->value(), Property::EraserSize};
}

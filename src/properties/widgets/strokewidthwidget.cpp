#include "strokewidthwidget.h"

#include "../property.h"
#include <QSpinBox>

StrokeWidthWidget::StrokeWidthWidget(QWidget *parent) : PropertyWidget{parent} {
    QSpinBox *box{new QSpinBox(parent)};
    box->setRange(1, 10);
    box->setValue(6);

    box->hide();
    m_widget = box;

    QObject::connect(box, &QSpinBox::valueChanged, this, [this]() { emit changed(value()); });
}

QString StrokeWidthWidget::name() const {
    return "Thickness";
};

const Property StrokeWidthWidget::value() const {
    return Property{dynamic_cast<QSpinBox *>(m_widget)->value(), Property::StrokeWidth};
}
